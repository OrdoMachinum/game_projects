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


float HybridFloatLength(const dtHybridLength * const h)
{
    return h->solidus * GRID_SCALE + h->articlus;
}

/* v1 - v2 */
void HybridSubtract(
    const dtHybridMagnitudeVector * const v1,
    const dtHybridMagnitudeVector * const v2,
    dtHybridMagnitudeVector * const v1v2)
{
    v1v2->K.X = v1->K.X - v2->K.X;
    v1v2->K.Y = v1->K.Y - v2->K.Y;
    v1v2->q.x = v1->q.x - v2->q.x;
    v1v2->q.y = v1->q.y - v2->q.y;
}

/* v1 + v2 */
void HybridAdd(
    const dtHybridMagnitudeVector * const v1,
    const dtHybridMagnitudeVector * const v2,
    dtHybridMagnitudeVector * const v1v2)
{
    v1v2->K.X = v1->K.X + v2->K.X;
    v1v2->K.Y = v1->K.Y + v2->K.Y;
    v1v2->q.x = v1->q.x + v2->q.x;
    v1v2->q.y = v1->q.y + v2->q.y;
}

void HybridScale(
    dtHybridMagnitudeVector * const v, 
    const float scale)
{
    float fK = scale * v->K.X;
    float fq = scale * v->q.x;

    fq += fmodf(fK, GRID_SCALE) * GRID_SCALE;
    v->K.X = ROUND_M(fK);
    v->q.x = fq;

    fK = scale * v->K.Y;
    fq = scale * v->q.y;

    fq += fmodf(fK, GRID_SCALE) * GRID_SCALE;
    v->K.Y = ROUND_M(fK);
    v->q.y = fq;
}

void HybridToVec2(
    const dtHybridMagnitudeVector * const inHybridPosition,
    Vector2 * const vec2)
{
    vec2->x = inHybridPosition->K.X * GRID_SCALE + inHybridPosition->q.x;
    vec2->y = inHybridPosition->K.Y * GRID_SCALE + inHybridPosition->q.y;
}

void HybVectOrdnung(
    dtHybridMagnitudeVector * const v)
{
    if( GRID_SCALE < fabsf(v->q.x) ) {
        v->K.X += ROUND_M(v->q.x / GRID_SCALE);
        v->q.x -= fmodf(v->q.x, GRID_SCALE) * GRID_SCALE;
    }

    if( GRID_SCALE < fabsf(v->q.y) ) {
        v->K.Y += ROUND_M(v->q.y / GRID_SCALE);
        v->q.y -= fmodf(v->q.y, GRID_SCALE) * GRID_SCALE;
    }
}
