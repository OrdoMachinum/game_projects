#include "game.h"
#include <string.h>

int Game::m_gameInstanceNumber{0};

Game::Game(const std::string & windowTitle, const size_t windowWidth, const size_t windowHeight, const size_t targetFPS)
    : m_scrWidth{windowWidth},
      m_scrHeight{windowHeight},
      m_targetFPS{targetFPS},
      m_windowTitle{windowTitle}
{
    if(m_gameInstanceNumber) {
        throw "Only one game instance is allowed at a time";
    }
    m_gameInstanceNumber++;
}

bool Game::Initialize()
{
    bool fine{false};

    InitWindow(m_scrWidth,m_scrHeight, m_windowTitle.c_str());
    SetTargetFPS(m_targetFPS);
    m_camera.zoom = 7.e-7f;
    m_camera.offset.x = m_scrWidth/2;
    m_camera.offset.y = m_scrHeight/2;

    fine |= IsWindowReady();

    m_mainScreenMonoFont = LoadFontEx("resources/Terminus.ttf",fontHeight, NULL, 0);

    fine |= IsFontValid(m_mainScreenMonoFont);

    // Example earth-moon system
    AddCelestial({0,0,VectorCoordinateSystem::Cartesian2d},{0.f, 0.f, VectorCoordinateSystem::Cartesian2d}, M_EARTH, R_EARTH);
    AddCelestial({0,D_EARTH_MOON,VectorCoordinateSystem::Cartesian2d},{V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, 5.*M_MOON, R_MOON);
    AddCelestial({0,-D_EARTH_MOON,VectorCoordinateSystem::Cartesian2d},{-V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, M_MOON, R_MOON);


    return fine;
}

void Game::RunLoop()
{
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        ProcessInput();
        UpdateGameWorld();
        GenerateOutput();
    }
}


void Game::GenerateOutput()
{
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(m_camera);
    DrawCelestials();
    EndMode2D();

    DrawUI();


    EndDrawing();
}

void Game::AddCelestial(
    const WorldVector & position,
    const WorldVector & velocity,
    const float mass,
    const float radius,
    const std::string & name,
    const Color & color)
{
    m_system.push_back({position, velocity, mass, radius});
    m_sysView.push_back({name, color});

    m_gravityMatrix.push_back(std::vector<WorldVector>(m_system.size()));


    for (size_t i = 0; i < m_system.size()-1; ++i) {
        m_gravityMatrix.at(i).push_back({0., 0.});
    }

}

void Game::DrawCelestials()
{
    for (size_t ip = 0u; ip < m_system.size(); ++ip) {

        const Celestial& planet{m_system.at(ip)};        

        // Draw planets
        DrawCircleV({static_cast<float>(planet.position.q1), static_cast<float>(planet.position.q2)}, planet.radius, m_sysView.at(ip).getColor());


        switch(static_cast<Indicators>(m_indicator)){
            case (Indicators::Velocity) : {
                // Draw arrows pointing as the veolocity vectors
                DrawAWorldVector(planet.velocity, planet.position, 1e5);
                break;
            }
            case (Indicators::StrongestInteraction) : {
                // Draw line to most significant interaction
                int msignificant{planet.underTheStrongestInflunceOf};
                Color cl{DARKGRAY};
                cl.a *= 0.4;
                DrawLineV(
                    {static_cast<float>(planet.position.q1), static_cast<float>(planet.position.q2)},
                    {static_cast<float>(m_system[msignificant].position.q1), static_cast<float>(m_system[msignificant].position.q2)},
                    cl);        
            }
        }
    }
}

void Game::DrawAWorldVector(const WorldVector & headVector, const WorldVector & originVector, const float scale)
{
    const WorldVector pointOfArrow {scale*headVector + originVector};
    WorldVector leftHead{(0.9 * scale * headVector).rotate(0.05) + originVector};
    WorldVector rightHead{(0.9 * scale * headVector).rotate(-0.05) + originVector};
    
    DrawLineV(
        {static_cast<float>(originVector.q1), static_cast<float>(originVector.q2)},
        {static_cast<float>(pointOfArrow.q1), static_cast<float>(pointOfArrow.q2)},
        RED
    );
    
    DrawTriangleLines(
        {static_cast<float>(pointOfArrow.q1), static_cast<float>(pointOfArrow.q2)},
        {static_cast<float>(leftHead.q1), static_cast<float>(leftHead.q2)},
        {static_cast<float>(rightHead.q1), static_cast<float>(rightHead.q2)},
        RED
    );   
}

void DrawAScreenVector(const Vector2 & head, const Vector2 & origin)
{
    DrawLine(origin.x, origin.y, head.x, head.y, RED);
}

void Game::DrawUI()
{
    
    PrintLabels();
    switch(static_cast<Info>(m_info)) {
        case (Info::GravInteractionMatrix) : {
            PrintGravityMatrix();
            break;
        }
        case (Info::Masses): {
            PrintPlanetInfos();
            break;
        }
        default:
            break;  
    }
}

void Game::CalcGravityMatrix()
{
    // For optimamalizing the gravitational force calculations, we use the Newton3 here
    // and collect the common forces into a skew-symmetric matrix
    // As F_12 = -F_21
    //
    //  0       F_01   F_02
    // -F_01    0      F_12
    // -F_02   -F_12   0

    for (size_t i = 0u; i < m_system.size(); ++i) {
        for (size_t j = i+1; j < m_system.size(); ++j) {
            m_gravityMatrix[i][j] = ForceGrav_ij(i,j);
            m_gravityMatrix[j][i] = (-1.) * m_gravityMatrix[i][j] ;
        }
    }
}

void Game::PrintGravityMatrix()
{
    memset(m_bufScreenText, 0, textBufSize);
    DrawFPS(0,0);
    Vector2 textPos = {30,30};
    size_t ic{0};

    for (size_t i = 0u; i < m_system.size() && (ic < textBufSize-3); ++i) {
        for (size_t j = 0; j < m_system.size() && (ic < textBufSize-3); ++j) {
            
            ic += sprintf(m_bufScreenText+ic, "%+2.2e ", m_gravityMatrix[i][j].q2);
        }
        ic += sprintf(m_bufScreenText+ic,"\n");

    }
    sprintf(m_bufScreenText+ic,"\n");

    m_bufScreenText[textBufSize-1]=0;
    DrawTextEx(m_mainScreenMonoFont, m_bufScreenText, textPos, fontHeight,1,GREEN);
}

void Game::PrintPlanetInfos()
{
    memset(m_bufScreenText, 0, textBufSize);
    Vector2 textPos = {m_scrWidth/2, 30};
    size_t ic{0};

    for (size_t i = 0u; i < m_system.size() && (ic < textBufSize-3); ++i) {
        const Celestial& iP{m_system[i]};

        ic += sprintf(m_bufScreenText+ic,
        "%2d# %2.2e kg, strongest interaction with: %2d#\n", 
        iP.id(), iP.mass, iP.underTheStrongestInflunceOf);   
    }

    m_bufScreenText[textBufSize-1]=0;
    DrawTextEx(m_mainScreenMonoFont, m_bufScreenText,textPos, fontHeight,1,YELLOW);   
    //memset(m_bufScreenText, 0, textBufSize);
}

void Game::PrintLabels()
{
    char label[4]{0};
    for(const auto & planet : m_system) {
        Vector2 scr {GetWorldToScreen2D({planet.position.q1, planet.position.q2}, m_camera)};
        //scr.y -= 30;
        char label[4]{0};
        sprintf (label, "%3d", planet.id());
        DrawTextEx(m_mainScreenMonoFont,label,scr,20,1,BLUE);
    }
}

void Game::PrintHelp()
{
    
}

WorldVector Game::ForceGrav_ij(const size_t i, const size_t j)
{
    WorldVector dR_ij{m_system[i].position - m_system[j].position};
    LENGTH_TYPE distance{dR_ij.length()};

    return -GAMMA*m_system[i].mass*m_system[j].mass * (1./ distance/distance/distance ) * dR_ij ;
}

void Game::Newton2()
{
    for (size_t i = 0u; i < m_system.size(); ++i) {

        Celestial& iP{m_system[i]};

        // WorldVector netAcceleration{(1./iP.mass) * VecVecSum(m_gravityMatrix[i])};
        WorldVector netAcceleration{(1./iP.mass) * VecVecSum(m_gravityMatrix[i], iP.underTheStrongestInflunceOf)};

        iP.velocity = iP.velocity + m_deltaTime * netAcceleration;

        iP.position = iP.position + m_deltaTime * iP.velocity;

    }
}




void Game::ProcessInput()
{
    const float mWheel{GetMouseWheelMove()};

    // Process view related inputs


    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/m_camera.zoom);
        m_camera.target = Vector2Add(m_camera.target, delta);
    }
    if(mWheel) {
        m_camera.zoom *= exp(mWheel/15.75f);
    }
    
    if(IsKeyPressed(KEY_F1)){
        m_info = (++m_info)%(static_cast<int>(Info::LAST));
    }
    if(IsKeyPressed(KEY_F2)){
        m_indicator = (++m_indicator)%(static_cast<int>(Indicators::LAST));
    }
    

    // Process model related inputs


    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {

        Vector2 PointerInModelWorld {GetScreenToWorld2D(GetMousePosition(), m_camera)};
        WorldVector newPlanetLoc{PointerInModelWorld.x, PointerInModelWorld.y, VectorCoordinateSystem::Cartesian2d};
        AddCelestial(newPlanetLoc,{-V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, M_MOON, R_MOON);
    }


}

void Game::UpdateGameWorld()
{
    CalcGravityMatrix();
    Newton2();
}



void Game::ShutDown()
{
    CloseWindow();        // Close window and OpenGL context
}
