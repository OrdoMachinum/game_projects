#include "inputs.h"


int keyPressed;
float mWheel;
Vector2 mouseP;


void collectInputs(void) 
{
    keyPressed = GetKeyPressed();
    mWheel = GetMouseWheelMove();
    mouseP = GetMousePosition();
}

void processInputsForView(void) 
{
    if(mWheel) {
        currentView.pixelPerMeter *= exp(mWheel/15.75f);
    }
    
    switch (keyPressed)
    {
    case KEY_KP_ADD:
        currentView.pixelPerMeter *= 1.1;
        break;
    case KEY_KP_SUBTRACT:
        currentView.pixelPerMeter *= 0.9;
        break;
    case KEY_LEFT_SHIFT:
        frameTimeScale *= 1.1;
        break;
    case KEY_LEFT_CONTROL:
        frameTimeScale *= 0.9;
        break;
    case KEY_ENTER:
        iPlanetInFocus = (iPlanetInFocus+1u)%getNumPlanets();
        currentView.centerFOVinWorld = &(ppBodies[iPlanetInFocus]->position);
        break;
    default:
        break;
    }
    if (frameTimeScale < simultionScaling) {
        frameTimeScale = simultionScaling;
    }
}