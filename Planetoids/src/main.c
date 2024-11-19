#define _GNU_SOURCE
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "planets.h"

#include "view.h"
#include "physics.h"
#include "inputs.h"

const char * defaultSystem = "systems/tauri.csv";

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    unsigned long err = NO_ERROR;
    char * systemFileName = defaultSystem;

    if(err = initSystem()) {
        return err;        
    }

    if (1 == argc) {
        printf("No system was given, so default system is loaded: tauri.csv\n");
    } else if(2 == argc) {
        systemFileName = *(argv+1);
        printf("... file to be read: %s\n", systemFileName);
    }

    if(err = readSystemFromFile(systemFileName,";")) {
        return err;
    }

    CalcInitEnergies();
    initView(systemFileName);
    initSimulation(FPS);

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float simFrameTime = 0.f;

        // Collect Input
        //----------------------------------------------------------------------------------
        collectInputs();
        processInputsForView();
        
        // Update Model
        //----------------------------------------------------------------------------------
        delTick = 0u; // To check how many full system update is done for the next frame
        do{
            // In this loop we use the fact that the required animation frameTime allows more 
            // physical system update. In this way simulation step could remain
            // intependent of the framerate, so the numerical instability culd be elminated.
            simFrameTime += deltaTime_s;

            Newton2(deltaTime_s);  // Physical world update
            delTick++;
            
        }while (simFrameTime < GetFrameTime() * frameTimeScale);
        
        scaledElapsedTime += simFrameTime;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(BLACK);

            DrawFPS(screenWidth-6*fontHeight, screenHeight-30);

            DrawPlanets(&currentView);

            ShowGUI();
                        
        }
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