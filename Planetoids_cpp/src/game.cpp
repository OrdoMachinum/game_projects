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
    InitWindow(m_scrWidth,m_scrHeight, m_windowTitle.c_str());
    SetTargetFPS(m_targetFPS);
    m_camera.zoom = 7.e-7f;
    m_camera.offset.x = m_scrWidth/2;
    m_camera.offset.y = m_scrHeight/2;

    // Example earth-moon system
    AddCelestial({0,0,VectorCoordinateSystem::Cartesian2d},{0.f, 0.f, VectorCoordinateSystem::Cartesian2d}, M_EARTH, R_EARTH);
    AddCelestial({0,D_EARTH_MOON,VectorCoordinateSystem::Cartesian2d},{V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, 5.*M_MOON, R_MOON);
    AddCelestial({0,-D_EARTH_MOON,VectorCoordinateSystem::Cartesian2d},{-V_MOON, 0.f, VectorCoordinateSystem::Cartesian2d}, M_MOON, R_MOON);


    return IsWindowReady();
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

        Celestial& planet{m_system.at(ip)};

        DrawCircleV({static_cast<float>(planet.position.q1), static_cast<float>(planet.position.q2)}, planet.radius, m_sysView.at(ip).getColor());

    }
}

void Game::DrawAWorldVector(const WorldVector & showVector, const WorldVector & originVector)
{
    DrawLine(originVector.q1, originVector.q2, showVector.q1, showVector.q2, RED);
}

void Game::DrawUI()
{
    PrintGravityMatrix();
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
    for (size_t i = 0u; i < m_system.size(); ++i) {
        for (size_t j = 0; j < m_system.size(); ++j) {
            //std::cout << m_gravityMatrix[i][j].q2 << "\t";
            printf("%+2.2e ", m_gravityMatrix[i][j].q2);
        }
        printf("\n");
        //std::cout << "\n";

    }
    printf("\n");
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

        WorldVector netAcceleration{(1./iP.mass) * VecVecSum(m_gravityMatrix[i])};

        iP.velocity = iP.velocity + m_deltaTime * netAcceleration;

        iP.position = iP.position + m_deltaTime * iP.velocity;

    }
}




void Game::ProcessInput()
{
    const float mWheel{GetMouseWheelMove()};

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/m_camera.zoom);
        m_camera.target = Vector2Add(m_camera.target, delta);
    }


    // Process view related inputs
    if(mWheel) {
        m_camera.zoom *= exp(mWheel/15.75f);
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
