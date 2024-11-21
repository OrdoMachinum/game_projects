#ifndef HYBRIDPOS_H
#define HYBRIDPOS_H

#include <stdint.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

#define GRID_SCALE ((float)(1.496e9f)) // Closest exact float32 number to 0.1 Astrtonomical Unit
#define INV_GRID_SCALE ((float)(1.f/GRID_SCALE)) 
#define SQRT_GRID_SCALE ((float)(sqrtf(GRID_SCALE))) 

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
void HybridToVec2(const dtHybridMagnitudeVector * const inHybridPosition, Vector2 * const vec2);

void HybridLengthSqr(const dtHybridMagnitudeVector * const hybVect, dtHybridLength * const hSquareLength);
void SqrtHybridLength(const dtHybridLength * const hybLength, dtHybridLength * const hSquareRoot);

float HybridFloatLength(const dtHybridLength * const h);
void HybridSubtract(const dtHybridMagnitudeVector * const v1, const dtHybridMagnitudeVector * const v2, dtHybridMagnitudeVector * const v1v2);
void HybridAdd(const dtHybridMagnitudeVector * const v1, const dtHybridMagnitudeVector * const v2, dtHybridMagnitudeVector * const v1v2);
void HybridScale(dtHybridMagnitudeVector * const v, const float scale);

void HybVectOrdnung(dtHybridMagnitudeVector * const v);

bool HybPosEquals(const dtHybridMagnitudeVector * const a, const dtHybridMagnitudeVector * const b);


#endif