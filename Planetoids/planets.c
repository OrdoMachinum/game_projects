#include "include/planets.h"

struct planet planets[3] = {
    {   // EARTH
        .mass = 6.0e24f,
        .radius = 6371e3f,
        .movable = true,
        .position = ORIG_2,
        .summedForce = ORIG_2,
        .velocity = ORIG_2,
        .color = GREEN,
    },
    {   // MOON
        .mass = 187.35e22f,
        .radius = 1737.4e3f,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = 384400e3f
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = 1.022e3f,
            .y = 0.f
            },
        .color = RAYWHITE,
    },
    {   // MOON2
        .mass = 7.35e22f,
        .radius = 1737.4e3f,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = 284400e3f
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = -2.022e3f,
            .y = 0.f
            },
        .color = BLUE,
    }
 };