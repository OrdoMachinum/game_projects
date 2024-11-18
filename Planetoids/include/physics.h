#ifndef PHYSICS_H
#define PHYSICS_H

#include "planets.h"
#include "raylib.h"
#include "raymath.h"


extern float sysFullEnergyInit;
extern float sysFullEnergy;
extern uint64_t trailTickNum;
extern float scaledElapsedTime;
extern float deltaTime_s;
extern const float simultionScaling;

void initSimulation(const uint8_t framePerSecundum);

void Newton2(const float deltaTs);
void Newton2WithThreads(const float deltaTs);

float KineticEnergy (const dtMassPoint * const body);
float GravPot(const Vector2 * const position);
void updateEnergyOfBody(dtMassPoint * const body);

void calcSysFullEnergy(void);

void CalcInitEnergies(void);



#endif