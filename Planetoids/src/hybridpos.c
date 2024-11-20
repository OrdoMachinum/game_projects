#include "hybridpos.h"

#define ROUND_M   floorf


void Vec2ToHybrid(
    const Vector2 * const inVec, 
    dtHybridMagnitudeVector * const hybridPosition)
{
    hybridPosition->K.X = ROUND_M(INV_GRID_SCALE * inVec->x);
    hybridPosition->q.x = inVec->x - hybridPosition->K.X;

    hybridPosition->K.Y = ROUND_M(INV_GRID_SCALE * inVec->y);
    hybridPosition->q.y = inVec->y - hybridPosition->K.Y;
}

void HybridLengthSqr(
    const dtHybridMagnitudeVector * const hybVect,
    dtHybridLength * const hSquareLength)
{
    const float qSquare = hybVect->q.x * hybVect->q.x + hybVect->q.y * hybVect->q.y;                // [m^2]
    const float qKMixed = hybVect->K.X * hybVect->q.x + hybVect->K.Y * hybVect->q.y;                // [m*AU]
    const float kOnly = (hybVect->K.X * hybVect->K.X + hybVect->K.Y * hybVect->K.Y);     // [AU^2]

    hSquareLength->solidus = ROUND_M(INV_GRID_SCALE*qSquare);
    hSquareLength->articlus = fmodf(qSquare,GRID_SCALE);
    
    hSquareLength->solidus += ROUND_M(qKMixed);
    hSquareLength->articlus += fmodf(qKMixed, 1.f) * GRID_SCALE;

    hSquareLength->solidus += ROUND_M(GRID_SCALE*kOnly);
    // GRID_SCALE and the Ks are integers!
}

void SqrtHybridLength(const dtHybridLength * const hybLength, dtHybridLength * const hSquareRoot)
{
    if(0 == hybLength->solidus) {
        hSquareRoot->solidus = 0;
        hSquareRoot->articlus = sqrtf(hybLength->articlus);
    } else {
        float q = INV_GRID_SCALE * hybLength->articlus;
        float sqrtK = sqrtf(hybLength->solidus);
        float invSqrtK = 1.f / sqrtK;

        /* Series expansion of sqrt(K + q) around K, considering q << K*/
        float rt = sqrtK;                               // 0.
        rt += 0.5f * invSqrtK * q;                      // 1.
        rt -= 0.125 * q*q * invSqrtK*invSqrtK*invSqrtK; // 2.

        rt *= SQRT_GRID_SCALE;

        hSquareRoot->solidus = ROUND(rt * INV_GRID_SCALE);
        hSquareRoot->articlus = fmodf(rt,GRID_SCALE);
    }
}