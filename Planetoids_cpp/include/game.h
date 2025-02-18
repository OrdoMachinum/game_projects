#ifndef GAME_H
#define GAME_H

#include <cstdio>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "solarconstants.h"
#include "worldvector.h"
#include "celestial.h"
#include "celestialview.h"

constexpr size_t textBufSize{1024};
constexpr float fontHeight{18};
constexpr float fontWidth{fontHeight * 0.55f};

enum class Info {
    None=0,
    GravInteractionMatrix,
    Masses,
    LAST,
};

enum class Indicators {
    None=0,
    StrongestInteraction,
    Velocity,
    LAST,
};

class Game
{
public:
    Game(const std::string& windowTitle, const size_t windowWidth=800, const size_t windowHeight=600, const size_t targetFPS=60);

    Game(const Game& src) = delete;
    Game(const Game&& src) = delete;

    Game& operator=(const Game& rhs) = delete;
    Game&& operator=(const Game&& rhs) = delete;

    bool Initialize();
    void RunLoop();
    void ShutDown();

private:
    void ProcessInput();
    void UpdateGameWorld();
    void GenerateOutput();

    void AddCelestial(const WorldVector& position, const WorldVector& velocity,  const float mass, const float radius, const std::string& name="", const Color& color=WHITE);

    void DrawCelestials();
    void DrawAWorldVector(const WorldVector & headVector, const WorldVector & originVector, const float scale=1.f);
    
    void DrawUI();

    void CalcGravityMatrix();
    void CalcConstrainsMatrix();
    void CalcCollisionCelestialsMatrix();

    WorldVector ForceGrav_ij(const size_t i, const size_t j);
    WorldVector ForcePairConstrain_ij(const size_t i, const size_t j, const LENGTH_TYPE distConst);
    void Newton2();


    void MergeCelestials();
    void SplitCelestial();


    void PrintGravityMatrix(const size_t xScr=0, const size_t yScr=0);
    void PrintPlanetInfos(const size_t xScr=0, const size_t yScr=0);
    void PrintLabels(const size_t siz=20);    
    void PrintHelp(const size_t xScr=0, const size_t yScr=0);
    void PrintZoomLevel(const size_t xScr=0, const size_t yScr=0);
    void PrintCoordinates(const size_t xScr=0, const size_t yScr=0);

    void DrawGrid(const Color& gridColor=DARKGRAY);

    int m_info{0};
    int m_indicator{0};

    size_t m_scrWidth{};
    size_t m_scrHeight{};
    size_t m_targetFPS{};

    std::array<char, textBufSize> m_bufScreenText{0};

    Font m_mainScreenMonoFont{};
    size_t m_fontSize{static_cast<size_t>(fontHeight)};

    Camera2D m_camera{};
    size_t m_centerOnObject{0};

    double m_deltaTime{10.*3600.0/m_targetFPS};

    std::vector<Celestial> m_system;
    std::vector<CelestialView> m_sysView;

    std::vector<std::vector<WorldVector> > m_gravityMatrix;
    std::vector<std::vector<WorldVector> > m_pairConstrainMatrix;
    std::vector<std::vector<bool> > m_collisionCelestialMatrix;

    std::string m_windowTitle{};

    static int m_gameInstanceNumber;

};

#endif // GAME_H
