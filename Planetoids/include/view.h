#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "planets.h"
#include "physics.h"
#include "inputs.h"
#include "hybridpos.h"
#include "vecdouble.h"

#define LINE_LENGTH (250)
#define FPS     (60u)

typedef struct dtView_tag{
    dtVector2 * centerFOVinWorld;
    Vector2 screenCenter;
    float pixelPerMeter;
    float pixelPerMeterPlanets;
}dtView;

extern dtView currentView;
extern float frameTimeScale;
extern uint16_t iPlanetInFocus;
extern const int fontHeight;
extern const int fontWidth;
extern const int screenWidth;
extern const int screenHeight;
extern Font inFont;
extern uint32_t delTick; // To check how many full system update is done for the next frame


void initView(const char * systemFileName);

void DrawPlanets(const dtView* const fov);

void DrawGrid_Here(const dtView* const fov);

void ShowGUI(void);

void printDats(FILE * const gp, const dtMassPoint * pl);

dtVector2 toRealCoord(const Vector2 * const screenCoord, const dtView* const fov);


#endif
