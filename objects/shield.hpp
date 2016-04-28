#ifndef SHIELD_HPP
#define SHIELD_HPP

#include "rectangle.hpp"
#include "..\stateman\gameEngine.hpp"

class Shield : public Rectangle
{
public:
    Shield();
    ~Shield();

    void Init(GameEngine *game);

    void Draw();

    bool IsEnabled() const noexcept { return m_isEnabled; }
    void SetEnabled(const bool state) noexcept { m_isEnabled = state; }
    sf::Vector2f GetSize() const noexcept { return m_size; }

private:
    GameEngine *m_engine;

    bool m_isEnabled;
    sf::Vector2f m_size;

    static const int HEIGHT;
    static const int ELEVATION;
    static const sf::Color COLOUR;
};

#endif // SHIELD_HPP
