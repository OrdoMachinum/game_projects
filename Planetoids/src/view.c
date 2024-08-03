#include "view.h"


struct view currentView = {
    .pixelPerMeter = 3.e-9,
    .pixelPerMeterPlanets = 3.e-9,
    .centerFOV = 0,
    .screenCenter ={0.f, 0.f},
};

void DrawPlanets(
    const struct view * const fov,
    const struct planet * const bodies)
{
    for(uint32_t i = 0u; i < getNumPlanets(); ++i) {

        Vector2 scr = Vector2Scale(
            Vector2Subtract(
            *(fov->centerFOV), bodies[i].position ),
            fov->pixelPerMeter);
        
        scr = Vector2Add(scr, fov->screenCenter);
        
        DrawCircleV(
            scr,
            planets[i].radius * fov->pixelPerMeterPlanets,
            planets[i].color);
    }   
}