#include "shield.hpp"

const int Shield::m_widthDefault = 430;
const int Shield::m_heightDefault = 2;

Shield::Shield(const float x, const float y) :
    m_width(m_widthDefault),
    m_height(m_heightDefault),
    m_isEnabled(true)
{
    shape.setPosition(x, y);
    shape.setSize({m_width, m_height});
    shape.setFillColor({255, 255, 255});
    shape.setOrigin(0, 0);
}

Shield::~Shield()
{

}
