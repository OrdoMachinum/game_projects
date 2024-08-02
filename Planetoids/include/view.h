#ifndef VIEW_H
#define VIEW_H

#include "planets.h"
#include <raylib.h>
#include <raymath.h>

struct view {
    Vector2 centerFOV;
    Vector2 screenCenter;
    float pixelPerMeter;
};

extern struct view currentView;


void DrawPlanets(const struct view * const fov, const struct planet * const bodies);

#endif
