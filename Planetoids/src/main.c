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
        case KEY_UP:
            currentView.centerFOV.y -= planets[0].radius;
            break;
        case KEY_DOWN:
            currentView.centerFOV.y +=  planets[0].radius;
            break;
        case KEY_LEFT:
            currentView.centerFOV.x -=  planets[0].radius;
            break;
        case KEY_RIGHT:
            currentView.centerFOV.x +=  planets[0].radius;
            break;
        case KEY_A:
            planets[0].mass += 5e25;
            break;
        case KEY_Y:
            planets[0].mass -= 5e21;
            break;
        case KEY_LEFT_SHIFT:
            currentView.pixelPerMeter *= 1.1;
            printf("pixelPerMeter:\t%e\n", currentView.pixelPerMeter);
            break;
        case KEY_LEFT_CONTROL:
            currentView.pixelPerMeter *= 0.9;
            printf("pixelPerMeter:\t%e\n", currentView.pixelPerMeter);
            break;
        case KEY_SPACE:
            currentView.centerFOV = planets[1].position;
            break;
        default:
            break;
        }

        if(IsKeyDown(KEY_SPACE)){
            currentView.centerFOV = planets[1].position;
        } else {
            currentView.centerFOV = planets[0].position;
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