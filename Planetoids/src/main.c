#define _GNU_SOURCE
#include <stdio.h>

#include "raylib.h"
#include "planets.h"
#include "raymath.h"
#include "view.h"
#include "physics.h"

#include <math.h>
#include <pthread.h>


#define FPS     (60u)

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
    const float simScale  = 1.f * 3600.f; // [s]
    float frameTimeScale = simScale * 24.f;
    const int fontHeight = 20;
    const int fontWidth  = fontHeight * 0.55f;
    float delT = 0.f;
    float scaledElapsedTime = 0.f;
    bool simulationWarning = false;
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
    CalcInitEnergies();

    //createRingOfBalls(D_SUN_MERCURY, R_JUPITER,M_JUPITER,14u);

    currentView.centerFOV = &(ppBodies[0]->position);

    printf("Number of bodies : %u\n", getNumPlanets());

    InitWindow(screenWidth, screenHeight, "Planetoids, refactored");

    Font inFont = LoadFontEx("resources/Terminus.ttf",fontHeight, NULL,0);

    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float simFrameTime = 0.f;


        // Collect Input
        //----------------------------------------------------------------------------------
        int key = GetKeyPressed();
        float mWheel = GetMouseWheelMove();
        
        if(mWheel) {
            currentView.pixelPerMeter *= exp(mWheel/100);
        }
        
        switch (key)
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
            iPlanet = (iPlanet+1u)%getNumPlanets();
            currentView.centerFOV = &(ppBodies[iPlanet]->position);
            break;
        default:
            break;
        }

        if (frameTimeScale < simScale) {
            frameTimeScale = simScale;
        }

        // Update Model
        //----------------------------------------------------------------------------------
        do{
            delT = simScale / FPS;
            simFrameTime += delT;

            Newton2(delT);

        }while (simFrameTime < GetFrameTime() * frameTimeScale);

        scaledElapsedTime += simFrameTime;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(BLACK);

            DrawFPS(screenWidth-6*fontHeight, screenHeight-30);

            DrawPlanets(&currentView);

            Vector2 textPos = {10,10};
            char textBuff[LINE_LENGTH] = {0};

            sprintf(textBuff, "Real time :\t%.1f day \t\t1 pixel = %2.1E km\nTime Step =\t%.1f s\nAnimation Speed =\t%3.1E week/frameSec. ",
                    scaledElapsedTime/24/3600,
                    1e3f/ currentView.pixelPerMeter,
                    delT,
                    frameTimeScale/7.f/24.f/3600.f);
            DrawTextEx(inFont, textBuff, textPos, fontHeight, 1, RAYWHITE);

            textPos.y = 10;

            memset(textBuff, 0, LINE_LENGTH);
            
            for(uint16_t pl = 0u; pl < getNumPlanets(); pl++){
                textBuff[0] = 0;
                dtMassPoint * pB = ppBodies[pl];
                updateEnergyOfBody(pB);
                textPos.x = screenWidth - fontWidth * ( 
                        sprintf(textBuff, 
                            "%c [%2d] mass : %3.3E kg\t DELTA-E: %+3.1E %%",(pl == iPlanet)? '>' : ' ',
                            pl, 
                            pB->mass,
                            100.f*(pB->initialEnergy - pB->currentEnergy)/pB->initialEnergy));
                DrawTextEx(inFont, textBuff, textPos, fontHeight, 1, pB->color);
                textPos.y += fontHeight;
            }

            calcSysFullEnergy();
            memset(textBuff, 0, LINE_LENGTH);
            textPos.x = screenWidth - fontWidth *
                        sprintf(textBuff, 
                            "System DELTA-E: %+4.1E %%",
                            100.f*(sysFullEnergyInit - sysFullEnergy)/sysFullEnergyInit);
            DrawTextEx(inFont, textBuff, textPos, fontHeight, 1, RED);

            memset(textBuff, 0, LINE_LENGTH);

            
            
            Vector2 mouseP = GetMousePosition();
            Vector2 pointerWorld = toRealCoord(&mouseP, &currentView);
            
            textPos.x = 10;
            sprintf(textBuff, 
                "Screen\tX: %.0f p Y: %.0f p\nReal\tX: %+4.1E m Y: %+4.1E m",
                mouseP.x, mouseP.y,
                pointerWorld, pointerWorld);

            DrawTextEx(inFont, textBuff, textPos, fontHeight, 1, GREEN);

            textPos.y = screenHeight - 4*fontHeight;
            
            DrawTextEx(inFont, " ENTER:\tchange planet\n left-SHIFT / left-CTRL:\tincrease/decrease animation speed\n + / -:\tzoom in/out\n ", textPos, fontHeight, 1, GRAY);
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