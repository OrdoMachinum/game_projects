#include "vecdouble.h"

void V2_add(const dtVector2 * const a, const dtVector2 * const b, dtVector2 * const aplusb)
{
    aplusb->x = a->x + b->x;
    aplusb->y = a->y + b->y;
}

void V2_sub(const dtVector2 * const a, const dtVector2 * const b, dtVector2 * const aminb)
{
    aminb->x = a->x - b->x;
    aminb->y = a->y - b->y;
}

double V2_length(const dtVector2 * const v)
{
    return sqrt(v->x * v->x + v->y * v->y);
}


void V2_scale(dtVector2 * const v, const double c)
{
    v->x *= c;
    v->y *= c;
}
