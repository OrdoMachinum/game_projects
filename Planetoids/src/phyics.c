#include"physics.h"

#include <pthread.h>

typedef struct {
    uint16_t iBodyStart; // Start index of calculation body in the case of multihreaded calculation 
    uint16_t iBodyNumber; // How many body is considered in this thread
    float   dTime;
} dtPhysThread;

pthread_t worker1;
pthread_t worker2;

uint64_t trailTickNum = 0;
float sysFullEnergyInit = 0.f;
float sysFullEnergy = 0.f;
const float simultionScaling  = 1.f * 3600.f; // [s]
float scaledElapsedTime = 0.f;

float deltaTime_s = 0.f;

void initSimulation(const uint8_t framePerSecundum)
{
    deltaTime_s = simultionScaling / framePerSecundum;
}

void CalcInitEnergies(void) 
{
    static bool inited = false;
    if(!inited) {
        for(uint32_t j = 0u; j < getNumPlanets(); j++){
            dtMassPoint * pB = *(ppBodies + j);
            updateEnergyOfBody(pB);
            pB->initialEnergy = pB->currentEnergy;
            sysFullEnergy += pB->currentEnergy;
        }
        sysFullEnergyInit = sysFullEnergy;
        inited = true;
    }
}


void calcSysFullEnergy(void) 
{
    sysFullEnergy = 0.f;
    for(uint32_t j = 0u; j < getNumPlanets(); j++){
        dtMassPoint * pB = *(ppBodies + j);
        updateEnergyOfBody(pB);
        sysFullEnergy += pB->currentEnergy;
    }
}

void updateEnergyOfBody(dtMassPoint * const body) 
{
    body->currentEnergy = KineticEnergy(body) + body->mass * GravPot(&body->position);
}

float KineticEnergy (const dtMassPoint * const body) 
{
    return 0.5f * Vector2LengthSqr(body->velocity) * body->mass;
}

float GravPot (
    const Vector2 * const position) 
{
    float potential = 0.f;
    for(uint32_t j = 0u; j < getNumPlanets(); j++){

        dtMassPoint * pB = *(ppBodies + j);

        if(Vector2Equals(pB->position, *position )){
            continue;
        }
        float r_ij = Vector2Distance(*position, pB->position);

        potential += (-GAMMA * pB->mass/r_ij);
    }
    return potential;
}

static void Gravity(
    const Vector2 * const position,
    Vector2 * const field)
{
    field->x = 0.f;
    field->y = 0.f;
    for(uint32_t i = 0u; i < getNumPlanets(); i++){
        dtMassPoint * pB = *(ppBodies + i);
        if(Vector2Equals(pB->position, *position )){
            continue;
        }
        Vector2 relVec = Vector2Subtract(*position, pB->position);
        Vector2 iForce = Vector2Normalize(relVec);
        iForce = Vector2Scale(iForce,
            (-GAMMA * pB->mass/Vector2DistanceSqr(*position, pB->position )) );
        *field = Vector2Add(*field, iForce);
    }
}

static void GravityWithHybridPosition(
    const dtHybridMagnitudeVector * const position,
    Vector2 * const field)
{
    field->x = 0.f;
    field->y = 0.f;
    for(uint32_t i = 0u; i < getNumPlanets(); i++){
        dtMassPoint * pB = *(ppBodies + i);
        HybVectOrdnung(&(pB->hPos));
        if(HybPosEquals(&(pB->hPos), position )){
            continue;
        }
        dtHybridMagnitudeVector relVecH;
        HybridSubtract(position, &(pB->hPos), &relVecH);

        dtHybridLength d_square_h;
        HybridLengthSqr(&relVecH, &d_square_h);
        float d_square = HybridFloatLength(&d_square_h);

        
        Vector2 iForce;
        HybridToVec2(&relVecH,&iForce);
        
        iForce = Vector2Normalize(iForce);

        iForce = Vector2Scale(iForce,
            (-GAMMA * pB->mass/d_square ));
        *field = Vector2Add(*field, iForce);
    }
}

void Newton2(
    const float deltaTs) 
{
    for(uint32_t i = 0u; i < getNumPlanets(); i++) {
        dtMassPoint * pB = *(ppBodies + i);

        if(!pB->movable) {
            continue;
        }
        Vector2 field;
        Gravity(&pB->position, &field);
        pB->velocity = Vector2Add(pB->velocity, Vector2Scale(field, deltaTs));
        pB->position = Vector2Add(pB->position, Vector2Scale(pB->velocity, deltaTs));
    }
}
void Newton2WithHybridPositioning(
    const float deltaTs)
{
    for(uint32_t i = 0u; i < getNumPlanets(); i++) {
        dtMassPoint * pB = *(ppBodies + i);

        if(!pB->movable) {
            continue;
        }
        Vector2 field;

        GravityWithHybridPosition(&pB->hPos, &field);

        pB->velocity = Vector2Add(pB->velocity, Vector2Scale(field, deltaTs));

        dtHybridMagnitudeVector hDeltaPos = {
            .K.X = 0,
            .K.Y = 0,
            .q.x = pB->velocity.x * deltaTs,
            .q.y = pB->velocity.y * deltaTs,
        };
        dtHybridMagnitudeVector hNewPos;

        HybridAdd(&(pB->hPos), &hDeltaPos, &hNewPos);
        memcpy(&(pB->hPos), &hNewPos, sizeof(dtHybridLength));

        HybridToVec2(&hNewPos, &(pB->position));
    }

}

    

void * N2ThreadWorker(void* n2Arg) 
{
    for(uint32_t i = ((dtPhysThread*)n2Arg)->iBodyStart;
        i < ((dtPhysThread*)n2Arg)->iBodyNumber + ((dtPhysThread*)n2Arg)->iBodyStart; 
        i++) {

        dtMassPoint * pB = *(ppBodies + i);

        if(!pB->movable) {
            continue;
        }
        Vector2 field;

        Gravity(&pB->position, &field);

        pB->velocity = Vector2Add(pB->velocity, Vector2Scale(field, ((dtPhysThread*)n2Arg)->dTime));
        
        pB->position = Vector2Add(pB->position, Vector2Scale(pB->velocity, ((dtPhysThread*)n2Arg)->dTime));
        
        if(pB->trail) {
            pB->trail[trailTickNum%TRAIL_LENGTH].position = pB->position;
            pB->trail[trailTickNum%TRAIL_LENGTH].alpha = (float)TRAIL_MAX_ALPHA;
        }
    }   

    return NULL;
}


void Newton2WithThreads(
    const float deltaT) 
{
    uint16_t partition = getNumPlanets() / 2;

    dtPhysThread w1 = {.dTime = deltaT, .iBodyNumber = getNumPlanets()/2, .iBodyStart = 0};
    dtPhysThread w2 = {.dTime = deltaT, .iBodyNumber = getNumPlanets() - partition, .iBodyStart =  partition};

    pthread_create(&worker1,NULL,N2ThreadWorker,&w1);
    pthread_create(&worker2,NULL,N2ThreadWorker,&w2);

    pthread_join(worker1, NULL);
    pthread_join(worker2, NULL);
    trailTickNum++;
    return;
}