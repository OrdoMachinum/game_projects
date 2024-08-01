#ifndef PLANETS_H
#define PLANETS_H

#include "raylib.h"
#include <stdint.h>


#define M_SUN           (1.989e30f) // kg
#define M_EARTH           (6.0e24f) // kg
#define M_MOON           (7.35e22f) // kg

#define R_SUN           (696340e3f) // m
#define R_EARTH           (6371e3f) // m
#define R_MOON          (1737.4e3f) // m

#define V_EARTH            (30.e3f) // m/s
#define V_MOON             (1023.f) // m/s

#define D_SUN_EARTH     (151.83e9f)
#define D_EARTH_MOON    (384400e3f)


#define ORIG_2  {0.f,   0.f}

struct planet {
    bool    movable;
    float   mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 summedForce;
    Color   color;
    float   radius;
};

extern struct planet planets[3];


#endif