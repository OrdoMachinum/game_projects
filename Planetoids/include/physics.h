#ifndef PHYSICS_H
#define PHYSICS_H

#include "raylib.h"
#include "raymath.h"

#include "planets.h"
#include "hybridpos.h"
#include "vecdouble.h"

void initSimulation(const uint8_t framePerSecundum);

void Newton2D(const float deltaTs);
/*
void Newton2(const float deltaTs);
void Newton2WithHybridPositioning(const float deltaTs);
void Newton2WithThreads(const float deltaTs);
*/
double KineticEnergy (const dtMassPoint * const body);
double GravPotD(const dtMassPoint * const body);
void updateEnergyOfBody(dtMassPoint * const body);

void calcSysFullEnergy(void);

void CalcInitEnergies(void);


extern double sysFullEnergyInit;
extern double sysFullEnergy;
extern uint64_t trailTickNum;
extern float scaledElapsedTime;
extern float deltaTime_s;
extern const float simultionScaling;

#endif