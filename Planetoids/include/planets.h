#ifndef PLANETS_H
#define PLANETS_H

#include "raylib.h"
#include <stdint.h>


#define M_SUN           (1.989e30f) // kg
#define M_EARTH           (6.0e24f) // kg
#define M_MOON           (7.35e22f) // kg
#define M_MERCURY      (3.3011e23f) // kg
#define M_VENUS        (4.8675e24f) // kg
#define M_MARS         (6.4171e23f) // kg
#define M_JUPITER      (1.8982e27f) // kg

#define R_SUN         (696340.0e3f) // m
#define R_EARTH         (6371.0e3f) // m
#define R_MOON          (1737.4e3f) // m
#define R_MERCURY       (2439.7e3f) // m
#define R_VENUS         (6051.8e3f) // m
#define R_MARS          (3389.5e3f) // m
#define R_JUPITER      (69911.0e3f) // m

#define V_EARTH         (30.0e3f) // m/s
#define V_MOON           (1.023e3f) // m/s
#define V_MERCURY       (47.37e3f) // m/s
#define V_VENUS         (35.02e3f) // m/s
#define V_MARS          (24.07e3f) // m/s
#define V_JUPITER       (13.07e3f) // m/s

#define D_SUN_EARTH     (151.83e9f)
#define D_EARTH_MOON    (384400e3f)

#define D_SUN_MERCURY   (57.91e9f)
#define D_SUN_VENUS   (108.21e9f)
#define D_SUN_MARS   (227939366e3f)
#define D_SUN_JUPITER   (778.479e9f)

#define GAMMA (6.674e-11f)

#define ORIG_2  {0.f,   0.f}

struct planet {
    bool    movable;
    float   mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 summedForce;
    Color   color;
    float   radius;
    Vector2 *trail;
};

extern struct planet planets[];

unsigned int getNumPlanets(void);



#endif