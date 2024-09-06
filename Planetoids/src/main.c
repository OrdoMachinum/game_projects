#define _GNU_SOURCE
#include <stdio.h>

#include "raylib.h"
#include "planets.h"
#include "raymath.h"
#include "view.h"
#include "physics.h"


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    unsigned long err = NO_ERROR;
    const int screenWidth = 1250;
    const int screenHeight = 1000;
    float timeScale  = 24.f * 3600.f; // simulationTime[s] /  frameTime [s]
    int fontSize = 20;
    float delTFrame = 0.f;
    currentView.screenCenter.x = screenWidth*0.5f;
    currentView.screenCenter.y = screenHeight*0.5f;
    currentView.centerFOV = NULL;
    
    uint16_t iPlanet = 0u;    

    err = initSystem();
    if(err) {
        return err;
    }

    err = readSystemFromFile("systems/tauri.csv", ";");
    if(err) {
        return err;
    }
    currentView.centerFOV = &(ppBodies[0]->position);

    printf("Number of bodies : %u\n", getNumPlanets());

    InitWindow(screenWidth, screenHeight, "Planetoids, refactored");

    Font inFont = LoadFontEx("resources/Terminus.ttf",fontSize, NULL,0);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        // Update
        int key = GetKeyPressed();

        switch (key)
        {
        case KEY_KP_ADD:
            currentView.pixelPerMeter *= 1.1;
            break;
        case KEY_KP_SUBTRACT:
            currentView.pixelPerMeter *= 0.9;
            break;
        case KEY_LEFT_SHIFT:
            timeScale *= 1.1;
            break;
        case KEY_LEFT_CONTROL:
            timeScale *= 0.9;
            break;
        case KEY_ENTER:
            iPlanet = (iPlanet+1u)%getNumPlanets();
            currentView.centerFOV = &(ppBodies[iPlanet]->position);
            break;
        default:
            break;
        }

        delTFrame = GetFrameTime() * timeScale;

        Newton2(delTFrame);
        updateSystem(delTFrame);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth-6*fontSize, screenHeight-30);

            DrawPlanets(&currentView);

            Vector2 textPos = {10,10};
            char textBuff[LINE_LENGTH] = {0};

            sprintf(textBuff, "Real time :\t%.1f day \t\t1 pixel = %2.1E km", GetTime(), 1e3f/ currentView.pixelPerMeter);
            DrawTextEx(inFont, textBuff, textPos, fontSize, 1, RAYWHITE);
            textPos.y = 10;
            textBuff[0] = 0;

            
            for(uint16_t pl = 0u; pl < getNumPlanets(); pl++){
                dtMassPoint * pB = ppBodies[pl];
                textPos.x = screenWidth - 0.6*fontSize*sprintf(textBuff, "%c [%2d] mass : %3.3E kg", (pl == iPlanet)? '>' : ' ', pl, pB->mass) ;
                DrawTextEx(inFont, textBuff, textPos, fontSize, 1, pB->color);
                textPos.y += fontSize;
            }

            textPos.y = screenHeight - 4*fontSize;
            textPos.x = 10;
            DrawTextEx(inFont, " ENTER:\tchange planet\n a / y:\tincrease/decrease mass\n + / -:\tzoom in/out\n ", textPos, fontSize, 1, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    err = destroySystem();
    return err;
}