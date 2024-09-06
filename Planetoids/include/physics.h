#ifndef PHYSICS_H
#define PHYSICS_H


#include "planets.h"
#include <raylib.h>
#include <raymath.h>


void updateSystem(const float deltaTs);
void Newton2(const float deltaTs);
float kineticEnergy (const dtMassPoint * const body);


#endif