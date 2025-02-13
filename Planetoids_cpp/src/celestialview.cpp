#include "celestialview.h"


CelestialView::CelestialView(const std::string& initialName, const Color& initialColor) :
    m_name{initialName}, m_color{initialColor}
{

}

std::string_view CelestialView::getName() const
{
    return m_name;
}

void CelestialView::setName(const std::string & newName)
{
    m_name = newName;
}

Color& CelestialView::getColor()
{
    return m_color;
}
