#ifndef PHYSICS_H
#define PHYSICS_H


#include "planets.h"
#include <raylib.h>
#include <raymath.h>
#include <pthread.h>


void Newton2(const float deltaTs);
void Newton2WithThreads(const float deltaTs);

float KineticEnergy (const dtMassPoint * const body);
float GravPot(const Vector2 * const position);
void updateEnergyOfBody(dtMassPoint * const body);

void calcSysFullEnergy(void);

void CalcInitEnergies(void);

extern float sysFullEnergyInit;
extern float sysFullEnergy;
extern uint64_t trailTickNum;



#endif