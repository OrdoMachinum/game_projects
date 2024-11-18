#ifndef INPUTS_H
#define INPUTS_H

#include "raylib.h"
#include "view.h"

extern int keyPressed;
extern float mWheel;
extern Vector2 mouseP;


void collectInputs(void);
void processInputsForView(void); 

#endif