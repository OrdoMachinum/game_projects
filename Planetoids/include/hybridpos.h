#ifndef HYBRIDPOS_H
#define HYBRIDPOS_H

#include <stdint.h>
#include "raylib.h"
#include "raymath.h"

#define GRID_SCALE ((float)(1.496e10f)) // Closest exact float32 number to 0.1 Astrtonomical Unit
#define INV_GRID_SCALE ((float)(1.f/1.496e10f)) 
#define SQRT_GRID_SCALE ((float)(122311.078811f)) 

typedef struct dtHybridLength_t {
    int32_t solidus;        // Closest big-range point's id
    float   articlus;       // Distance from .solidus
} dtHybridLength;

typedef struct dtFloatCoord_t {
    float x;
    float y;
} dtFloatCoord;

typedef struct dtIntegerCoord_t {
    int32_t X;
    int32_t Y;
} dtIntegerCoord;

typedef struct dtHybridMagnitudePosition_t {
    dtIntegerCoord K; 
    dtFloatCoord q;
} dtHybridMagnitudeVector;

void Vec2ToHybrid(const Vector2 * const inVec, dtHybridMagnitudeVector * const hybridPosition);

void HybridLengthSqr(const dtHybridMagnitudeVector * const hybVect, dtHybridLength * const hSquareLength);
void SqrtHybridLength(const dtHybridLength * const hybLength, dtHybridLength * const hSquareRoot);


#endif