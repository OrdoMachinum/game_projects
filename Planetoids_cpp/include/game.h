#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>

#include <vector>
#include <raylib.h>

#include <raylib.h>
#include <raymath.h>

#include <cstdio>

#include "solarconstants.h"
#include "worldvector.h"

#include "worldvector.h"
#include "celestial.h"
#include "celestialview.h"

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
    void DrawAWorldVector(const WorldVector& showVector, const WorldVector& originVector);
    void DrawUI();

    void CalcGravityMatrix();
    WorldVector ForceGrav_ij(const size_t i, const size_t j);
    void Newton2();

    void PrintGravityMatrix();

    size_t m_scrWidth{};
    size_t m_scrHeight{};
    size_t m_targetFPS{};

    Camera2D m_camera{};
    size_t m_centerOnObject{0};

    double m_deltaTime{10.*3600.0/m_targetFPS};

    std::vector<Celestial> m_system;
    std::vector<CelestialView> m_sysView;

    std::vector<std::vector<WorldVector> > m_gravityMatrix;

    std::string m_windowTitle{};

    static int m_gameInstanceNumber;

};

#endif // GAME_H
