#ifndef PLANETS_H
#define PLANETS_H

#define _GNU_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "errorID.h"

#define SYS_INCREMENT       (40u)
#define MAXNAME_LENGTH      (35)

#define TRAIL_LENGTH        (2000u)
#define TRAIL_MAX_ALPHA        (156)

#define SCALE_RADIUS            (1e3f) /*km*/
#define SCALE_POSITION          (1e9f) /*million km*/
#define SCALE_VELOCITY          (1e3f) /*km/s*/

#define GAMMA (6.674e-11f)
#define ORIG_2  {0.f,   0.f}

#define M_SUN           (1.989e30f) // kg
#define M_EARTH           (6.0e24f) // kg
#define M_MOON           (7.35e22f) // kg
#define M_MERCURY      (3.3011e23f) // kg
#define M_VENUS        (4.8675e24f) // kg
#define M_MARS         (6.4171e23f) // kg
#define M_JUPITER      (1.8982e27f) // kg
#define M_SATURN       (5.6834e26f) // kg
#define M_URANUS       (8.6810e25f) // kg
#define M_NEPTUNE     (1.02409e26f) // kg
#define M_PLUTO        (1.3025e22f) // kg

#define R_SUN         (696340.0e3f) // m
#define R_EARTH         (6371.0e3f) // m
#define R_MOON          (1737.4e3f) // m
#define R_MERCURY       (2439.7e3f) // m
#define R_VENUS         (6051.8e3f) // m
#define R_MARS          (3389.5e3f) // m
#define R_JUPITER      (69911.0e3f) // m
#define R_SATURN       (58232.0e3f) // m
#define R_URANUS       (25362.0e3f) // m
#define R_NEPTUNE      (24622.0e3f) // m
#define R_PLUTO         (1188.3e3f) // m

#define V_EARTH         (30.0e3f) // m/s
#define V_MOON           (1.023e3f) // m/s
#define V_MERCURY       (47.37e3f) // m/s
#define V_VENUS         (35.02e3f) // m/s
#define V_MARS          (24.07e3f) // m/s
#define V_JUPITER       (13.07e3f) // m/s
#define V_SATURN         (9.68e3f) // m/s
#define V_URANUS         (6.80e3f) // m/s
#define V_NEPTUNE        (5.43e3f) // m/s
#define V_PLUTO          (4.743e3f) // m/s

#define D_SUN_EARTH     (151.83e9f)
#define D_EARTH_MOON    (384400e3f)

#define D_SUN_MERCURY       (57.91e9f)
#define D_SUN_VENUS        (108.21e9f)
#define D_SUN_MARS      (227939366e3f)
#define D_SUN_JUPITER     (778.479e9f)
#define D_SUN_SATURN      (1433.53e9f)
#define D_SUN_URANUS    (2.870972e12f)
#define D_SUN_NEPTUNE        (4.5e12f)
#define D_SUN_PLUTO      (5.90638e12f)

typedef struct dtTrace_t {
    Vector2 position;
    float   alpha;
}dtTrace;

typedef struct dtPolar2_t {
    float r;
    float t;
}dtPolar2;

typedef struct dtPlanet_t{
    unsigned long int ID;
    char    name[MAXNAME_LENGTH];
    bool    simulate;
    bool    movable;
    float   mass;
    Vector2 position;
    Vector2 velocity;
    float   initialEnergy;
    float   currentEnergy;
    Color   color;
    float   radius;
    dtTrace * trail;
}dtMassPoint;

extern dtMassPoint ** ppBodies;

dtErrorID initSystem(void);
dtErrorID addBody(const dtMassPoint * const pBody);
dtErrorID destroySystem(void);

dtErrorID readSystemFromFile(const char * const fName, const char * delimiter);

dtMassPoint ** createRingOfBalls(
    const Vector2 center,
    const float radiusOfCircle,
    const float radiusOfBall,
    const float mass,
    const unsigned int num);

dtPolar2 convToPolar2(const Vector2 * const cartesian);
Vector2 convToCartesian2(const dtPolar2 * const polar);

unsigned int getNumPlanets(void);

#endif