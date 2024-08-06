#ifndef VIEW_H
#define VIEW_H

#include "planets.h"
#include <raylib.h>
#include <raymath.h>

#define LINE_LENGTH (250)

struct view {
    Vector2 * centerFOV;
    Vector2 screenCenter;
    float pixelPerMeter;
    float pixelPerMeterPlanets;
};

extern struct view currentView;


void DrawPlanets(const struct view * const fov, const struct planet * const bodies);

#endif
