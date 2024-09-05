#include "view.h"


#define INDICATOR_RADIUS_PX (20)

dtView currentView = {
    .pixelPerMeter = 3.e-9,
    .pixelPerMeterPlanets = 3.e-9,
    .centerFOV = 0,
    .screenCenter ={0.f, 0.f},
};

Vector2 toScreenCoord(const Vector2 * const realCoord, const dtView* const fov) 
{
    return Vector2Add(fov->screenCenter, 
    Vector2Scale(
                Vector2Subtract( *(fov->centerFOV), *realCoord),
            fov->pixelPerMeter) );
}

void DrawPlanets(
    const dtView* const fov,
    const dtMassPoint * const bodies)
{
    static unsigned int trailTick = 0u;

    for(uint32_t i = 0u; i < getNumPlanets(); ++i) {
        
        dtMassPoint * pB = bodies + i;

        Vector2 scr = toScreenCoord(&(pB->position), fov);

        if(pB->radius * fov->pixelPerMeter < 1.f ) {
            Color cl = pB->color;
            DrawPixelV(scr, cl);
            cl.a *= 0.3;
            DrawCircleLinesV(scr, INDICATOR_RADIUS_PX, cl);
        } else {
            DrawCircleV(scr, (pB->radius * fov->pixelPerMeter), pB->color);
        }
               

        if(NULL != pB->trail) {
            for (uint16_t tr = 0u; tr < TRAIL_LENGTH; tr++) {
                Color traceColor = pB->color;
                traceColor.a = (unsigned char)pB->trail[tr].alpha;
                scr = toScreenCoord(&(pB->trail[tr].position), fov);
                DrawPixelV(scr, traceColor);
                pB->trail[tr].alpha -= 255.f/TRAIL_LENGTH;
            }
        }
    }
    trailTick = (trailTick+1) % TRAIL_LENGTH;
}



void printDats(FILE * const fp, const dtMassPoint * pl)
{     
    fprintf(fp, "%4.2e\n", kineticEnergy(pl) );
    fflush(fp);
}