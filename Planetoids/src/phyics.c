#include"physics.h"

static uint64_t tickNum = 0;
float sysFullEnergyInit = 0.f;
float sysFullEnergy = 0.f;

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

void Newton2(
    const float deltaT) 
{
    for(uint32_t i = 0u; i < getNumPlanets(); i++) {
        dtMassPoint * pB = *(ppBodies + i);

        if(!pB->movable) {
            continue;
        }
        Vector2 field;
        Gravity(&pB->position, &field);
        pB->velocity = Vector2Add(pB->velocity, Vector2Scale(field, deltaT));
    }   
}

void updateSystem(
    const float deltaT)
{
    for(uint32_t i = 0u; i < getNumPlanets() ; i++) {
        dtMassPoint * pB = *(ppBodies + i);
        if(!pB->movable) {
            continue;
        }
        pB->position = Vector2Add(pB->position, Vector2Scale(pB->velocity, deltaT));
        
        if(pB->trail) {
            pB->trail[tickNum%TRAIL_LENGTH].position = pB->position;
            pB->trail[tickNum%TRAIL_LENGTH].alpha = (float)TRAIL_MAX_ALPHA;
        }
    }
    tickNum++;
}