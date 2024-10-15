#ifndef PHYSICS_H
#define PHYSICS_H


#include "planets.h"
#include <raylib.h>
#include <raymath.h>


void updateSystem(const float deltaTs);
void Newton2(const float deltaTs);
float KineticEnergy (const dtMassPoint * const body);
float GravPot(const Vector2 * const position);
float EnergySum(const dtMassPoint * const body);

void CalcInitEnergies(void);


#endif