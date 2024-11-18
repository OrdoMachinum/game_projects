#include "view.h"


#define INDICATOR_RADIUS_PX (20)

dtView currentView = {
    .pixelPerMeter = 3.e-9,
    .pixelPerMeterPlanets = 3.e-9,
    .centerFOVinWorld = NULL,
    .screenCenter ={0.f, 0.f},
};


float frameTimeScale = 0;
const int fontHeight = 20;
const int fontWidth  = fontHeight * 0.55f;
const int screenWidth = 1250;
const int screenHeight = 1000;
uint32_t delTick = 0u; // To check how many full system update is done for the next frame
uint16_t iPlanetInFocus = 0u;
Font inFont = {0};




void initView(const char * systemFileName)
{
    frameTimeScale = simultionScaling * 24.f;
    currentView.screenCenter.x = screenWidth*0.5f;
    currentView.screenCenter.y = screenHeight*0.5f;
    currentView.centerFOVinWorld = &(ppBodies[0]->position);

    InitWindow(screenWidth, screenHeight, systemFileName);
    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    inFont = LoadFontEx("resources/Terminus.ttf",fontHeight, NULL,0);
}

Vector2 toScreenCoord(const Vector2 * const realCoord, const dtView* const fov) 
{
    return Vector2Add(fov->screenCenter, 
    Vector2Scale(
                Vector2Subtract( *(fov->centerFOVinWorld), *realCoord),
            fov->pixelPerMeter) );
}

Vector2 toRealCoord(const Vector2 * const screenCoord, const dtView* const fov) 
{
    Vector2 w = Vector2Subtract(fov->screenCenter, *screenCoord);

    w = Vector2Scale(w, 1.f/fov->pixelPerMeter);

    w = Vector2Add(*(fov->centerFOVinWorld), w);
    return w;
}

void DrawPlanets(
    const dtView* const fov)
{
    for(uint32_t i = 0u; i < getNumPlanets(); ++i) {
        
        dtMassPoint * pB = *(ppBodies + i);

        Vector2 scr = toScreenCoord(&(pB->position), fov);

        if(pB->radius * fov->pixelPerMeter < 1.f ) {
            Color cl = WHITE;
            cl.a *= 0.9;
            DrawPixelV(scr, cl);
            cl = RED;
            cl.a *= 0.3;
            DrawCircleLinesV(scr, INDICATOR_RADIUS_PX, cl);
        } else {
            DrawCircleV(scr, (pB->radius * fov->pixelPerMeter), pB->color);
        }
               

        if(NULL != pB->trail) {

            uint64_t iTrailcurrent = trailTickNum%TRAIL_LENGTH;
            uint64_t iTrailNext = (trailTickNum+1)%TRAIL_LENGTH;
         
            pB->trail[iTrailcurrent].position = pB->position;
            pB->trail[iTrailcurrent].alpha = (float)TRAIL_MAX_ALPHA;
        

            for (uint16_t tr = 0u; tr < TRAIL_LENGTH-1; tr++) {
                Color traceColor = pB->color;
                traceColor.a = (unsigned char)pB->trail[tr].alpha;
                scr = toScreenCoord(&(pB->trail[tr].position), fov);

                /* The float conversion here is really important, if we want a working fade effect. */
                pB->trail[tr].alpha -= ((float)TRAIL_MAX_ALPHA)/TRAIL_LENGTH; 

                if(tr == iTrailcurrent || tr == iTrailNext) {
                    continue;
                }

                DrawLineV(
                    toScreenCoord(&(pB->trail[tr].position), fov),
                    toScreenCoord(&(pB->trail[tr+1].position), fov), traceColor);
                //DrawPixelV(scr, traceColor);
                
            }
        }
    }
    trailTickNum++;
}

void ShowGUI(void)
{
              Vector2 textPos = {10,10};
            char textBuff[LINE_LENGTH] = {0};

            sprintf(textBuff, 
            "Real time :\t%.1f day \t\t1 pixel = %2.1E km\nTime Step =\t%.1f s\nAnimation Speed =\t%3.1E week/frameSec.\n System Update in a frame : %d\n",
                    scaledElapsedTime/24/3600,
                    1e3f/ currentView.pixelPerMeter,
                    deltaTime_s,
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
                            "%c [%-12s] mass : %3.3E kg\t ",(pl == iPlanetInFocus)? '>' : ' ',
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



void printDats(FILE * const fp, const dtMassPoint * pl)
{     
    fprintf(fp, "%4.2e\n", KineticEnergy(pl) );
    fflush(fp);
}