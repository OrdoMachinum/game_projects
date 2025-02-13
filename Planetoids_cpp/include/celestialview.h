#ifndef CELESTIALVIEW_H
#define CELESTIALVIEW_H

#include <string>
#include <raylib.h>
#include <string_view>
#include <array>
#include "worldvector.h"

class CelestialView
{
public:
    CelestialView(const std::string& initialName, const Color& initialColor);

    std::string_view getName() const;
    void setName(const std::string& newName);

    Color& getColor();
    void setColor(const Color& newColor);

    std::array<WorldVector, 4000> trace;

private:

    std::string m_name;
    Color m_color;
};

#endif // CELESTIALVIEW_H
