#include "raylib.h"
#include "planets.h"
#include "raymath.h"
#include "view.h"
#include "solsystem.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1250;
    const int screenHeight = 1000;
    const float timeScale  = 24.f * 3600.f; // simulationTime[s] /  frameTime [s]
    float delTFrame = 0.f;
    currentView.screenCenter.x = screenWidth*0.5f;
    currentView.screenCenter.y = screenHeight*0.5f;
    currentView.centerFOV = &(planets[0].position);
    uint16_t iPlanet = 0u;


    printf("Number of planets : %u\n", getNumPlanets());

    InitWindow(screenWidth, screenHeight, "Planetoids, refactored");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        // Update
        int key = GetKeyPressed();


        switch (key)
        {

        case KEY_A:
            planets[0].mass *= 1.2;
            break;
        case KEY_Y:
            planets[0].mass *= 0.8;
            break;
        case KEY_KP_ADD:
            currentView.pixelPerMeter *= 1.1;
            break;
        case KEY_KP_SUBTRACT:
            currentView.pixelPerMeter *= 0.9;
            break;
        case KEY_LEFT_SHIFT:
            currentView.pixelPerMeterPlanets *= 1.1;
            break;
        case KEY_LEFT_CONTROL:
            currentView.pixelPerMeterPlanets *= 0.9;
            break;
        case KEY_ENTER:
            iPlanet = (iPlanet+1u)%getNumPlanets();
            printf("%u planet\n",iPlanet);
            currentView.centerFOV = &(planets[iPlanet].position);
            break;
        default:
            break;
        }



        delTFrame = GetFrameTime() * timeScale;

        Newton2(delTFrame, planets);
        updateSystem(delTFrame, planets);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawPlanets(&currentView, planets);
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}