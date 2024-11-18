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

const char * defaultSystem = "systems/tauri.csv";

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
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
    currentView.centerFOVinWorld = NULL;
    char * systemFileName = defaultSystem;
    uint16_t iPlanet = 0u;
         

    err = initSystem();
    if(err) {
        return err;        
    }

    if (1 == argc) {
        printf("No system was given, so default system is loaded: tauri.csv\n");
    } else if(2 == argc) {
        systemFileName = *(argv+1);
        printf("... file to be read: %s\n", systemFileName);
    }

    err = readSystemFromFile(systemFileName,";");
    if(err) {
        return err;
    }
    CalcInitEnergies();

    //createRingOfBalls(D_SUN_MERCURY, R_JUPITER,M_JUPITER,14u);

    currentView.centerFOVinWorld = &(ppBodies[0]->position);

    printf("Number of bodies : %u\n", getNumPlanets());

    InitWindow(screenWidth, screenHeight, systemFileName);

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
            currentView.pixelPerMeter *= exp(mWheel/37.5f);
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
            currentView.centerFOVinWorld = &(ppBodies[iPlanet]->position);
            break;
        default:
            break;
        }

        if (frameTimeScale < simScale) {
            frameTimeScale = simScale;
        }

        // Update Model
        //----------------------------------------------------------------------------------
        uint32_t delTick = 0u; // To check how many full system update is done for the next frame
        do{
            // In this loop we use the fact that the required animation frameTime allows more 
            // physical system update. In this way simulation step could remain
            // intependent of the framerate, so the numerical instability culd be elminated.
            delT = simScale / FPS;
            simFrameTime += delT;

            Newton2(delT);  // Physical world update
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

            Vector2 textPos = {10,10};
            char textBuff[LINE_LENGTH] = {0};

            sprintf(textBuff, 
            "Real time :\t%.1f day \t\t1 pixel = %2.1E km\nTime Step =\t%.1f s\nAnimation Speed =\t%3.1E week/frameSec.\n System Update in a frame : %d\n",
                    scaledElapsedTime/24/3600,
                    1e3f/ currentView.pixelPerMeter,
                    delT,
                    frameTimeScale/7.f/24.f/3600.f,
                    delTick);
            DrawTextEx(inFont, textBuff, textPos, fontHeight, 1, RAYWHITE);

            textPos.y = 10;

            memset(textBuff, 0, LINE_LENGTH);
            
            for(uint16_t pl = 0u; pl < getNumPlanets(); pl++){
                textBuff[0] = 0;
                dtMassPoint * pB = ppBodies[pl];
                updateEnergyOfBody(pB);
                textPos.x = screenWidth - fontWidth *
                        sprintf(textBuff, 
                            "%c [%-12s] mass : %3.3E kg\t ",(pl == iPlanet)? '>' : ' ',
                            pB->name, 
                            pB->mass);
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
                pointerWorld.x, pointerWorld.y);

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