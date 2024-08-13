#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

#include <raylib.h>
#include <raymath.h>

#include "planets.h"
#include "physics.h"


#define LINE_LENGTH (250)

typedef struct dtView_tag{
    Vector2 * centerFOV;
    Vector2 screenCenter;
    float pixelPerMeter;
    float pixelPerMeterPlanets;
}dtView;

extern dtView currentView;


void DrawPlanets(const dtView* const fov, const dtMassPoint * const bodies);

void printDats(FILE * const gp, const dtMassPoint * pl);

#endif
