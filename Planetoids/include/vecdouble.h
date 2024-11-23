#ifndef VECDOUBLE_H
#define VECDOUBLE_H


typedef struct dtVector2_t {
    double x;
    double y;
} dtVector2;

void V2_add(const dtVector2 * const a, const dtVector2 * const b, dtVector2 * const aplusb);
void V2_sub(const dtVector2 * const a, const dtVector2 * const b, dtVector2 * const aminb);
double V2_length(const dtVector2 * const v);
void V2_scale(dtVector2 * const v, const double c);

#endif