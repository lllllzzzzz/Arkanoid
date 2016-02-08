#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "rectangle.hpp"

class Shield : public Rectangle
{
public:
    Shield(const float x, const float y);
    ~Shield();

    bool IsEnabled() const noexcept { return m_isEnabled; }
    void Enable() { m_isEnabled = true; }
    void Disable() { m_isEnabled = false; }
    int getWidth() const noexcept { return m_width; }
    int getHeight() const noexcept { return m_height; }

private:
    bool m_isEnabled;
    int m_width;
    int m_height;

    static const int m_widthDefault;
    static const int m_heightDefault;
};

#endif // SHIELD_HPP
