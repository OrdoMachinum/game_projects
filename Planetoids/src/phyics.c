#include"physics.h"

static uint64_t tickNum = 0;

float kineticEnergy (const dtMassPoint * const body) 
{
    return 0.5f * Vector2LengthSqr(body->velocity) * body->mass;
}

static void Gravity(
    const Vector2 * const position,
    const dtMassPoint * const bodies,
    Vector2 * const field)
{
    field->x = 0.f;
    field->y = 0.f;
    for(uint32_t i = 0u; i < getNumPlanets(); i++){

        dtMassPoint * pB = bodies + i;

        if(Vector2Equals(pB->position, *position )){
            continue;
        }
        Vector2 relVec = Vector2Subtract(*position, pB->position);
        Vector2 iForce =  Vector2Normalize (relVec);

        iForce = Vector2Scale(iForce,
            (-GAMMA * pB->mass/Vector2DistanceSqr(*position, pB->position )) );

        *field = Vector2Add(*field, iForce);
    }
}

void Newton2(
    const float deltaT,
    dtMassPoint * const bodies) 
{
    for(uint32_t i = 0u; i < getNumPlanets(); i++) {
        dtMassPoint * pB = bodies + i;

        if(!pB->movable) {
            continue;
        }
        Vector2 field;
        Gravity(&pB->position, bodies, &field);
        pB->velocity = Vector2Add(pB->velocity, Vector2Scale(field, deltaT));
    }   
}

void updateSystem(
    const float deltaT,
    dtMassPoint * const bodies)
{
    for(uint32_t i = 0u; i < getNumPlanets() ; i++) {
        dtMassPoint * pB = bodies + i;
        if(!pB->movable) {
            continue;
        }
        pB->position = Vector2Add(pB->position, Vector2Scale(pB->velocity, deltaT));
        
        pB->trail[tickNum%TRAIL_LENGTH].position = pB->position;
        pB->trail[tickNum%TRAIL_LENGTH].alpha = 255u;
    }
    tickNum++;
}