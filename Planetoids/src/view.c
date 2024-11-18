#include "view.h"


#define INDICATOR_RADIUS_PX (20)

dtView currentView = {
    .pixelPerMeter = 3.e-9,
    .pixelPerMeterPlanets = 3.e-9,
    .centerFOVinWorld = NULL,
    .screenCenter ={0.f, 0.f},
};

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
        

            for (uint16_t tr = 0u; tr < TRAIL_LENGTH; tr++) {
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
  // TBD
}



void printDats(FILE * const fp, const dtMassPoint * pl)
{     
    fprintf(fp, "%4.2e\n", KineticEnergy(pl) );
    fflush(fp);
}