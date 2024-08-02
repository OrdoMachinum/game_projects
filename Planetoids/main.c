
#include "raylib.h"
#include "raymath.h"
#include "include/planets.h"
#include <stdio.h>


#define PIX_TRAIL   (400)
#define GAMMA (6.674e-11f)

#define MIN(A, B) (A<B?A:B)

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Font font;

float time;


unsigned int tick = 0u;

Vector2 trail_A[PIX_TRAIL]  =  {0};
Vector2 trail_B[PIX_TRAIL]  =  {0};
Vector2 trail_C[PIX_TRAIL]  =  {0};

const int screenWidth = 1200;
const int screenHeight = 800;

int screenCenterX = screenWidth*0.5f;
int screenCenterY = screenHeight*0.5f;
float pixelPerMeter = 1.f;
float timeScale = 1.f;
Vector2 centerFOV = {0};
const Vector2 screenCenter =  {screenWidth*0.5, screenHeight*0.5};

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
int main(void)
{

    InitWindow(screenWidth, screenHeight, "planetoids");

    time = 0.f;
        printf("%d \n", 1000000);
    

    // Initialization
    //--------------------------------------------------------------------------------------

    //pixelPerMeter = MIN(screenHeight,screenWidth) / 384400e3f / 2.3f ;
    pixelPerMeter = 3.e-09;
    //pixelPerMeter = 1.5f / planets[1].radius;

    timeScale = 24.f * 3600.f; // screenSecundum/realSecundum
    printf("%d \n", 0);

    printf("%d \n", 10);
    font = LoadFontEx("resources/Terminus.ttf", 32, 0, 250);
    printf("%d \n", 11);

    planets[0].trail = trail_A;
    planets[1].trail = trail_B;
    planets[2].trail = trail_C;

    printf("%d \n", 12);
    //--------------------------------------------------------------------------------------

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (WindowShouldClose())    // Detect window close button or ESC key
    {

        tick++;

        //printf("%d \n", tick);

        int key = GetKeyPressed();
        switch (key)
        {
        case KEY_UP:
            centerFOV.y -= planets[0].radius;
            break;
        case KEY_DOWN:
            centerFOV.y +=  planets[0].radius;
            break;
        case KEY_LEFT:
            centerFOV.x -=  planets[0].radius;
            break;
        case KEY_RIGHT:
            centerFOV.x +=  planets[0].radius;
            break;
        case KEY_A:
            planets[0].mass += 5e25;
            break;
        case KEY_Y:
            planets[0].mass -= 5e21;
            break;
        case KEY_LEFT_SHIFT:
            pixelPerMeter *= 1.1;
            printf("pixelPerMeter:\t%e\n", pixelPerMeter);
            break;
        case KEY_LEFT_CONTROL:
            pixelPerMeter *= 0.9;
            printf("pixelPerMeter:\t%e\n", pixelPerMeter);
            break;
        case KEY_SPACE:
            centerFOV = planets[1].position;
            break;
        default:
            break;
        }

        if(IsKeyDown(KEY_SPACE)){
            centerFOV = planets[1].position;
        } else {
            centerFOV = planets[0].position;
        }
        UpdateDrawFrame();
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFont(font);
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
    //printVect2(&field);
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

        //printf("REA: %d:\tx=%.2f\ty=%.2f\n", i, planets[i].position.x, planets[i].position.y);
    }
}




static void DrawPlanets(void)
{
            time += GetFrameTime() * timeScale;

        printf("%f\n", pixelPerMeter);

    Vector2 tp = {10, 20};

    char st[150] = {0};

    //sprintf(st,
      //  "time = %3.1f day\t\t pixelPermeter = %2.3e\n", time, pixelPerMeter);

    //DrawTextEx(font, st,tp, (float)30, 3, GREEN);

    for(uint32_t i = 0u; i < 3; ++i) {
        for(uint32_t k = 0u; k < PIX_TRAIL; k++)
        {
            Vector2 trScr = Vector2Scale(
            Vector2Subtract(centerFOV, planets[i].trail[k]),
            pixelPerMeter);
            trScr = Vector2Add(trScr, screenCenter);
            DrawPixelV(trScr, planets[i].color);
        }

        Vector2 scr = Vector2Scale(
            Vector2Subtract(centerFOV, planets[i].position ),
            pixelPerMeter);
        
        scr = Vector2Add(scr, screenCenter);
        //realToScreen(&scr, &(planets[i].position));

        
      //  printf("REA: %d:\tx=%.2f\ty=%.2f\n", i, planets[i].position.x, planets[i].position.y);
      //  printf("SCR: %d:\tx=%.2f\ty=%.2f\n", i, scr.x, scr.y);
        DrawCircleV(
            scr,
            planets[i].radius * pixelPerMeter,
            planets[i].color);

        planets[i].trail[tick%PIX_TRAIL] = planets[i].position;        

        Vector2 deltaTextVector = {0,32};
        tp = Vector2Add(tp, deltaTextVector);

        //sprintf(st,
        //    "\ni:%d\tx = %-2.2em\ty=%-2.2em\n", i, planets[i].position.x, planets[i].position.y);
        //DrawTextEx(font, st,tp, (float)30, 3, GREEN);
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
