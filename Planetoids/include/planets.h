#ifndef PLANETS_H
#define PLANETS_H

#include "raylib.h"
#include <stdint.h>

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