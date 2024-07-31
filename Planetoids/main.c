
#include "raylib.h"
#include "raymath.h"
#include "include/planets.h"
#include <stdio.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define GAMMA (6.674e-11f)

#define MIN(A, B) (A<B?A:B)

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Camera camera = { 0 };
Vector3 cubePosition = { 0 };



const int screenWidth = 800;
const int screenHeight = 800;
float pixelPerMeter = 1.f;
float timeScale = 1.f;


//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame

static inline void printVect2(const Vector2 * const v) {
    printf("x = %10.3f\t y = %10.3f\n", v->x, v->y);
}

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    pixelPerMeter = MIN(screenHeight,screenWidth) / 384400e3f / 2.3f ;
    //pixelPerMeter = 1.5f / planets[1].radius;

    timeScale = 24.f * 3600.f; // screenSecundum/realSecundum

    InitWindow(screenWidth, screenHeight, "raylib");

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


static void Gravity(const Vector2 * const position, Vector2 * const field)
{
    field->x = 0.f;
    field->y = 0.f;
    for(uint32_t i = 0u; i < 3; i++){

        if(Vector2Equals(planets[i].position, *position )){
            continue;
        }

        Vector2 relVec = Vector2Subtract(*position, planets[i].position);
        Vector2 iForce =  Vector2Normalize (relVec);


        iForce = Vector2Scale(iForce,
            (-GAMMA*planets[i].mass/Vector2DistanceSqr(*position, planets[i].position )) );


        *field = Vector2Add(*field, iForce);
    }
    printVect2(&field);
}


static void Newton2(void) 
{
    for(uint32_t i = 0u; i < 3u ; i++) {
        if(!planets[i].movable) {
            continue;
        }
        Vector2 field;
        
        Gravity(&planets[i].position, &field);
        planets[i].velocity = Vector2Add(planets[i].velocity, Vector2Scale(field, GetFrameTime()*timeScale ));
    }   
}

static void UpdatePositions(void)
{
    for(uint32_t i = 0u; i < 3u ; i++) {
        if(!planets[i].movable) {
            continue;
        }
        planets[i].position.x += GetFrameTime() * timeScale * planets[i].velocity.x;
        planets[i].position.y += GetFrameTime() * timeScale * planets[i].velocity.y;

        printf("REA: %d:\tx=%.2f\ty=%.2f\n", i, planets[i].position.x, planets[i].position.y);
    }
}


static void realToScreen(
    Vector2 * const screenVec,
    const Vector2 * const realVec) 
{
    screenVec->x = realVec->x * pixelPerMeter + screenWidth * 0.5;
    screenVec->y = realVec->y * pixelPerMeter + screenHeight * 0.5;
}


static void DrawPlanets(void)
{
    //printf("%f\n", pixelPerMeter);
    for(uint32_t i = 0u; i < 3; ++i) {
        Vector2 scr = {0};
        realToScreen(&scr, &(planets[i].position));
      //  printf("REA: %d:\tx=%.2f\ty=%.2f\n", i, planets[i].position.x, planets[i].position.y);
      //  printf("SCR: %d:\tx=%.2f\ty=%.2f\n", i, scr.x, scr.y);
        DrawCircleV(
            scr,
            planets[i].radius * pixelPerMeter,
            planets[i].color);
    }
}



// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    //UpdateCamera(&camera, CAMERA_ORBITAL);
    //----------------------------------------------------------------------------------

    Newton2();
    UpdatePositions();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(BLACK);

        DrawPlanets();
        

        DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
