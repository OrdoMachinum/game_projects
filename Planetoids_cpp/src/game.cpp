#include "game.h"

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

    AddCelestial({0,-D_EARTH_MOON,VectorCoordinateSystem::Cartesian2d},{-0.7*V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, M_MOON, R_MOON);
    
    AddCelestial({0,-2.*D_EARTH_MOON,VectorCoordinateSystem::Cartesian2d},{-V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, M_MOON, R_MOON);

//    m_system.at(2).boundTo=3;
//    m_system.at(3).boundTo=2;

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
        
    DrawUI();

    BeginMode2D(m_camera);
    {
        DrawCelestials();
    }    
    EndMode2D();

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
    m_pairConstrainMatrix.push_back(std::vector<WorldVector>(m_system.size()));
    
    m_collisionCelestialMatrix.push_back(std::vector<bool>(m_system.size()));


    for (size_t i = 0; i < m_system.size()-1; ++i) {
        m_gravityMatrix.at(i).push_back({0., 0.});
        m_pairConstrainMatrix.at(i).push_back({0., 0.});
        m_collisionCelestialMatrix.at(i).push_back(false);
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
    size_t line{0};
    DrawFPS(0, m_fontSize*line++);
    PrintZoomLevel(0, m_fontSize*line++);
    PrintCoordinates(0,m_fontSize*line++);
    PrintHelp(0, m_scrHeight-30);
    PrintLabels(10);
    DrawGrid();

    switch(static_cast<Info>(m_info)) {
        case (Info::GravInteractionMatrix) : {
            PrintGravityMatrix(0,m_fontSize*line);
            break;
        }
        case (Info::Masses): {
            PrintPlanetInfos(0,m_fontSize*line);
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

void Game::CalcConstrainsMatrix()
{

    for (size_t i = 0u; i < m_system.size(); ++i) {
        if(m_system[i].boundTo < 0) {
            continue;
        }
        for (size_t j = i+1; j < m_system.size(); ++j) {
          
            m_pairConstrainMatrix[i][j] = ForcePairConstrain_ij(i, j, 0.5*D_EARTH_MOON);
            m_pairConstrainMatrix[j][i] = -1. * m_pairConstrainMatrix[i][j];
        }
    }
}

void Game::CalcCollisionCelestialsMatrix()
{
}


void Game::PrintGravityMatrix(const size_t xScr, const size_t yScr)
{
    m_bufScreenText.fill(0);
    char* buff{m_bufScreenText.data()};

    Vector2 textPos = {xScr, yScr};
    size_t ic{0};

    for (size_t i = 0u; i < m_system.size() && (ic < textBufSize-30); ++i) {
        for (size_t j = 0; j < m_system.size() && (ic < textBufSize-30); ++j) {
            
            ic += sprintf(buff+ic, "%+2.2e ", m_gravityMatrix[i][j].q2);
        }
        ic += sprintf(buff+ic,"\n");

    }
    sprintf(buff+ic,"\n");

    buff[textBufSize-1]=0;
    DrawTextEx(m_mainScreenMonoFont, buff, textPos, m_fontSize, 1, GREEN);
}

void Game::PrintPlanetInfos(const size_t xScr, const size_t yScr)
{
    m_bufScreenText.fill(0);
    char* buff{m_bufScreenText.data()};
    
    Vector2 textPos = {xScr, yScr};
    size_t ic{0};

    for (size_t i = 0u; i < m_system.size() && (ic < textBufSize-50); ++i) {
        const Celestial& iP{m_system[i]};

        ic += sprintf(buff+ic,
        "%2d# %2.2e kg, strongest interaction with: %2d#\n", 
        iP.id(), iP.mass, iP.underTheStrongestInflunceOf);   
    }

    buff[textBufSize-1]=0;
    DrawTextEx(m_mainScreenMonoFont, buff, textPos, m_fontSize,1,YELLOW);   
}

void Game::PrintLabels(const size_t siz)
{   
    for(const auto & planet : m_system) {
        Vector2 scrUnbound {GetWorldToScreen2D({planet.position.q1, planet.position.q2}, m_camera)};
        
        char label[4]{0};

        int numchar{sprintf (label, "%3d", planet.id())};

        Vector2 scr{Vector2Clamp(scrUnbound,{0,0},{m_scrWidth-(numchar*siz), m_scrHeight-2*siz})};

        DrawTextEx(m_mainScreenMonoFont, label, scr, m_fontSize, 1, BLUE);
    }
}

void Game::PrintHelp(const size_t xScr, const size_t yScr)
{
    Vector2 scr{xScr,yScr};
    std::string helpMessage{
        "F1: change displayed information, F2: change indicators, Right-Click: Add a moon"
    };
    DrawTextEx(m_mainScreenMonoFont, helpMessage.c_str(),scr, m_fontSize, 1, GRAY);
}

void Game::PrintZoomLevel(const size_t xScr, const size_t yScr)
{
    Vector2 textPos = {xScr, yScr};
    m_bufScreenText.fill(0);
    char* buff{m_bufScreenText.data()};
    sprintf(buff,"zoom: %2.2e m/px", 1.f/m_camera.zoom);
    DrawTextEx(m_mainScreenMonoFont,buff,textPos,m_fontSize,1,RED);
}

void Game::PrintCoordinates(
    const size_t xScr, 
    const size_t yScr)
{
    Vector2 textPos = {xScr, yScr};
    m_bufScreenText.fill(0);

    Vector2 mouseWrld{GetScreenToWorld2D(GetMousePosition(), m_camera)};

    sprintf(m_bufScreenText.data(),"x = %+1.2em,  y = %1.2em", mouseWrld.x, mouseWrld.y);

    DrawTextEx(m_mainScreenMonoFont,m_bufScreenText.data(),textPos,m_fontSize,1,RED);
}

void Game::DrawGrid(const Color& gridColor)
{
    const int tickDistance{D_EARTH_MOON*m_camera.zoom};
    Vector2 origo{GetWorldToScreen2D({0.,0.}, m_camera)};
    size_t tickSize{3};

    //origo = Vector2Clamp(origo,{2,2}, {static_cast<float>(m_scrWidth)-2, static_cast<float>(m_scrHeight)-2});
    
    DrawLine(0, origo.y, m_scrWidth, origo.y, gridColor);
    DrawLine(origo.x, 0, origo.x, m_scrHeight, gridColor);

    if(10>tickDistance) {
        return;
    }
    int k{100*m_scrWidth/tickDistance};

    for(int i{-k}; i < k; ++i){
        
        DrawLine(origo.x + tickDistance*i, origo.y-tickSize,
                 origo.x + tickDistance*i, origo.y+tickSize, gridColor);

        DrawLine(origo.x - tickSize, origo.y + tickDistance*i,
                 origo.x + tickSize, origo.y + tickDistance*i, gridColor);
    }
}

WorldVector Game::ForceGrav_ij(const size_t i, const size_t j)
{
    const WorldVector dR_ij{m_system[i].position - m_system[j].position};
    const LENGTH_TYPE distance{dR_ij.length()};

    return -GAMMA*m_system[i].mass*m_system[j].mass * (1./ distance/distance/distance ) * dR_ij ;
}

WorldVector Game::ForcePairConstrain_ij(const size_t i, const size_t j, const LENGTH_TYPE distConst)
{
    const Celestial &ip{m_system[i]};
    const Celestial &jp{m_system[j]};

    if(ip.boundTo < 0 || jp.boundTo < 0) {
        return {0., 0.};
    }

    const WorldVector dR_ij{ip.position - jp.position};
    const LENGTH_TYPE distance{dR_ij.length()};
    const LENGTH_TYPE strength{1e-3};
    const LENGTH_TYPE DELTA {distance-distConst};

    return WorldVector{-strength * (DELTA*DELTA*DELTA) * (1./distance) * dR_ij};
}

void Game::Newton2()
{
    for (size_t i = 0u; i < m_system.size(); ++i) {

        Celestial& iP{m_system[i]};

        WorldVector netForce{VecVecSum(m_gravityMatrix[i], iP.underTheStrongestInflunceOf)};

        netForce = netForce + VecVecSum(m_pairConstrainMatrix[i]);


        iP.velocity = iP.velocity + m_deltaTime * (1./iP.mass) * netForce;

        iP.position = iP.position + m_deltaTime * iP.velocity;

    }

}


void Game::MergeCelestials()
{
}

void Game::SplitCelestial()
{
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
    CalcConstrainsMatrix();

    Newton2();
}



void Game::ShutDown()
{
    CloseWindow();        // Close window and OpenGL context
}
