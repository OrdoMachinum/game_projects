#include "view.h"

#include <stdio.h>

#define INDICATOR_RADIUS_PX (20)

struct view currentView = {
    .pixelPerMeter = 3.e-9,
    .pixelPerMeterPlanets = 3.e-9,
    .centerFOV = 0,
    .screenCenter ={0.f, 0.f},
};

Vector2 toScreenCoord(const Vector2 * const realCoord, const struct view * const fov) 
{
    return Vector2Add(fov->screenCenter, 
    Vector2Scale(
                Vector2Subtract( *(fov->centerFOV), *realCoord),
            fov->pixelPerMeter) );
}

void DrawPlanets(
    const struct view * const fov,
    const struct planet * const bodies)
{
    static unsigned int trailTick = 0u;

    for(uint32_t i = 0u; i < getNumPlanets(); ++i) {
        Vector2 scr = toScreenCoord(&(bodies[i].position), fov);

        if(bodies[i].radius * fov->pixelPerMeter < 1.f ) {
            Color cl = bodies[i].color;
            DrawPixelV(scr, cl);
            cl.a *= 0.3;
            DrawCircleLinesV(scr, INDICATOR_RADIUS_PX, cl);
        } else {
            DrawCircleV(scr, (bodies[i].radius * fov->pixelPerMeter), bodies[i].color);
        }
               

        if(NULL != bodies[i].trail) {
            for (uint16_t tr = 0u; tr < TRAIL_LENGTH; tr++) {
                Color traceColor = bodies[i].color;
                traceColor.a = (unsigned char)bodies[i].trail[tr].alpha;
                scr = toScreenCoord(&(bodies[i].trail[tr].position), fov);
                DrawPixelV(scr, traceColor);
                bodies[i].trail[tr].alpha -= 255.f/TRAIL_LENGTH;
            }
        }
    }
    trailTick = (trailTick+1) % TRAIL_LENGTH;
}