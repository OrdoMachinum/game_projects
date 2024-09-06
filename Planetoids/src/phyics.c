#include"physics.h"

static uint64_t tickNum = 0;

float kineticEnergy (const dtMassPoint * const body) 
{
    return 0.5f * Vector2LengthSqr(body->velocity) * body->mass;
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