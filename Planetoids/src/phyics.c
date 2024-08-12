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
        if(Vector2Equals(bodies[i].position, *position )){
            continue;
        }
        Vector2 relVec = Vector2Subtract(*position, bodies[i].position);
        Vector2 iForce =  Vector2Normalize (relVec);

        iForce = Vector2Scale(iForce,
            (-GAMMA*bodies[i].mass/Vector2DistanceSqr(*position, bodies[i].position )) );

        *field = Vector2Add(*field, iForce);
    }
}

void Newton2(
    const float deltaT,
    dtMassPoint * const bodies) 
{
    for(uint32_t i = 0u; i < getNumPlanets(); i++) {
        if(!bodies[i].movable) {
            continue;
        }
        Vector2 field;
        Gravity(&bodies[i].position, bodies, &field);
        bodies[i].velocity = Vector2Add(bodies[i].velocity, Vector2Scale(field, deltaT));
    }   
}

void updateSystem(
    const float deltaT,
    dtMassPoint * const bodies)
{
    for(uint32_t i = 0u; i < getNumPlanets() ; i++) {
        if(!bodies[i].movable) {
            continue;
        }
        bodies[i].position = Vector2Add(bodies[i].position, Vector2Scale(bodies[i].velocity, deltaT));
        
        bodies[i].trail[tickNum%TRAIL_LENGTH].position = bodies[i].position;
        bodies[i].trail[tickNum%TRAIL_LENGTH].alpha = 255u;
    }
    tickNum++;
}