#ifndef SOLSYSTEM_H
#define SOLSYSTEM_H

#include "planets.h"
#include <raylib.h>
#include <raymath.h>


void updateSystem(const float deltaTs, struct planet * const bodies);
void Newton2(const float deltaTs, struct planet * const bodies);

#endif