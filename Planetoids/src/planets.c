#include "planets.h"




dtMassPoint planets[] = {
    {   // Sun
        .mass = M_SUN,
        .radius = R_SUN,
        .movable = true,
        .position = ORIG_2,
        .summedForce = ORIG_2,
        .velocity = ORIG_2,
        .color = YELLOW,
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
        .color = GRAY,
    },
    {   // Mercury
        .mass = M_MERCURY,
        .radius = R_MERCURY,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_MERCURY,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_MERCURY,
            .y = 0.f
            },        
            .color = GRAY,
    },
    {   // Venus
        .mass = M_VENUS,
        .radius = R_VENUS,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_VENUS,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_VENUS,
            .y = 0.f
            },        
            .color = ORANGE,
    },
    {   // MARS
        .mass = M_MARS,
        .radius = R_MARS,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_MARS,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_MARS,
            .y = 0.f
            },        
            .color = RED,
    },
    {   // JUPITER
        .mass = M_JUPITER,
        .radius = R_JUPITER,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_JUPITER,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_JUPITER,
            .y = 0.f
            },        
            .color = ORANGE,
    },
    {   // SATURN
        .mass = M_SATURN,
        .radius = R_SATURN,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_SATURN,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_SATURN,
            .y = 0.f
            },        
            .color = GOLD,
    },
    {   // URANUS
        .mass = M_URANUS,
        .radius = R_URANUS,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_URANUS,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_URANUS,
            .y = 0.f
            },        
            .color = BLUE,
    },
    {   // NEPTUNE
        .mass = M_NEPTUNE,
        .radius = R_NEPTUNE,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_NEPTUNE,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_NEPTUNE,
            .y = 0.f
            },        
            .color = DARKBLUE,
    },
    {   // PLUTO
        .mass = M_PLUTO,
        .radius = R_PLUTO,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_PLUTO,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_PLUTO,
            .y = 0.f
            },        
            .color = BROWN,
    },
 };



 unsigned int getNumPlanets(void) 
 {
    return (unsigned int)(sizeof(planets) / sizeof(planets[0]));
 }