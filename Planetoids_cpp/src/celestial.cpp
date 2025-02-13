#include "celestial.h"
using namespace std;

size_t Celestial::numOfCelestialBodies{0};


Celestial::Celestial(const WorldVector& initialPosition, const WorldVector& initialVelocity, const float initialMass, const float initialRadius) :
    m_id{numOfCelestialBodies},
    position{initialPosition},
    velocity{initialVelocity},
    mass{initialMass},
    radius{initialRadius}
{
    cout << "Celestial\t" << m_id << " is created.\n";
    numOfCelestialBodies++;
}

bool Celestial::operator==(const Celestial & compare) const
{
    return (this->m_id == compare.m_id);
}

size_t Celestial::id() const
{
    return m_id;
}
