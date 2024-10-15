#ifndef PLANETS_H
#define PLANETS_H

#define _GNU_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <raylib.h>
#include <raymath.h>

#include "errorID.h"

#define SYS_INCREMENT       (40u)
#define MAXNAME_LENGTH      (35)

#define TRAIL_LENGTH        (2000u)
#define TRAIL_MAX_ALPHA        (60)

#define SCALE_RADIUS            (1e3f) /*km*/
#define SCALE_POSITION          (1e9f) /*million km*/
#define SCALE_VELOCITY          (1e3f) /*km/s*/

#define GAMMA (6.674e-11f)

#define ORIG_2  {0.f,   0.f}

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