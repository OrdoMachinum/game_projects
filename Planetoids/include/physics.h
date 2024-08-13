#ifndef PHYSICS_H
#define PHYSICS_H


#include "planets.h"
#include <raylib.h>
#include <raymath.h>


void updateSystem(const float deltaTs, dtMassPoint * const bodies);
void Newton2(const float deltaTs, dtMassPoint * const bodies);
float kineticEnergy (const dtMassPoint * const body);


#endif