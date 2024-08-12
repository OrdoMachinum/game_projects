#define _GNU_SOURCE
#define _POSIX_SOURCE
#define GNUPLOT "gnuplot -persist"

#include "raylib.h"
#include "planets.h"
#include "raymath.h"
#include "view.h"
#include "physics.h"
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
    float timeScale  = 24.f * 3600.f; // simulationTime[s] /  frameTime [s]
    int fontSize = 20;
    float delTFrame = 0.f;
    currentView.screenCenter.x = screenWidth*0.5f;
    currentView.screenCenter.y = screenHeight*0.5f;
    currentView.centerFOV = &(planets[0].position);
    uint16_t iPlanet = 0u;    

    dtTrace trails[getNumPlanets()][TRAIL_LENGTH];

    FILE * fp = fopen("plot.dat", "w");
    
    if(!fp){
        return -1;
    }
    
    

    printf("Number of planets : %u\n", getNumPlanets());

    for(uint16_t ip = 0u; ip < getNumPlanets(); ip++){
       
        for (uint16_t it = 0; it < TRAIL_LENGTH; it++){
            trails[ip][it].position.x = NAN;
            trails[ip][it].position.y = NAN;
            trails[ip][it].alpha = 255.f;
        }
        
        planets[ip].trail = trails[ip];

        printf("trail %d addr: %p\n ", ip, planets[ip].trail);
    }


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

        case KEY_A:
            planets[iPlanet].mass *= 1.2;
            planets[iPlanet].radius *= 1.071;
            break;
        case KEY_Y:
            planets[iPlanet].mass *= 0.8;
            planets[iPlanet].radius *= 0.91;
            break;
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
            //printf("%u planet\n",iPlanet);
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

            DrawFPS(screenWidth-400, screenHeight-30);


            DrawPlanets(&currentView, planets);
            printDats(fp, &planets[3]);

            Vector2 textPos = {10,10};
            char textBuff[LINE_LENGTH] = {0};

            sprintf(textBuff, "Real time :\t%.1f day \t\t1 pixel = %2.1E km", GetTime(), 1e3f/ currentView.pixelPerMeter);
            DrawTextEx(inFont, textBuff, textPos, fontSize, 1, RAYWHITE);
            textPos.y = 10;
            textBuff[0] = 0;

            
            for(uint16_t pl = 0u; pl < getNumPlanets(); pl++){
                //printf(" %c [%d] mass : %f kg \n", ((pl == iPlanet)? '>' : ' '), pl, planets[pl].mass) ;
                textPos.x = screenWidth - 0.6*fontSize*sprintf(textBuff, "%c [%2d] mass : %3.3E kg", (pl == iPlanet)? '>' : ' ', pl, planets[pl].mass) ;
                DrawTextEx(inFont, textBuff, textPos, fontSize, 1, planets[pl].color);
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
    fclose(fp);
    //--------------------------------------------------------------------------------------

    return 0;
}