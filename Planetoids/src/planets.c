#include "planets.h"

struct planet planets[3] = {
    {   // Sun
        .mass = M_SUN,
        .radius = R_SUN,
        .movable = true,
        .position = ORIG_2,
        .summedForce = ORIG_2,
        .velocity = ORIG_2,
        .color = ORANGE,
    },
    {   // EARTH
        .mass = M_EARTH,
        .radius = R_EARTH,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_EARTH,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_EARTH,
            .y = 0.f
            },        
            .color = GREEN,
    },
    {   // MOON
        .mass = M_MOON,
        .radius = R_MOON,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_EARTH + D_EARTH_MOON,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_EARTH - V_MOON,
            .y = 0.f
            },
        .color = RAYWHITE,
    }
 };

 unsigned int getNumPlanets(void) 
 {
    return (unsigned int)(sizeof(planets) / sizeof(planets[0]));
 }