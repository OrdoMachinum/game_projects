#ifndef CELESTIAL_H
#define CELESTIAL_H
#include <cstddef>
#include <iostream>
#include "worldvector.h"

class Celestial
{
public:
    Celestial(const WorldVector& initialPosition, const WorldVector& initialVelocity, const float initialMass, const float initialRadius);

    bool operator==(const Celestial& compare) const;

    float mass{0};
    float radius{0};

    WorldVector position{0};
    WorldVector velocity{0};

    int underTheStrongestInflunceOf{-1};

    bool isMovable{true};

    size_t id() const;

protected:
    static size_t numOfCelestialBodies;
    size_t m_id{0};

};

#endif // CELESTIAL_H
