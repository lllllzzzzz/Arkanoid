#ifndef BRICK_HPP
#define BRICK_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "rectangle.hpp"
#include "..\stateman\gameEngine.hpp"

class Brick : public Rectangle
{
public:
    Brick(GameEngine *game, const sf::Vector2f position, const sf::Color colour, const int type);
    ~Brick();

    void Init(GameEngine *game);

    void SetPos(const sf::Vector2f newPosition);

    void Draw();

    const bool IsVisible() const noexcept;
    void SetVisibility(const bool isVisible) noexcept;

    int GetType() const noexcept { return m_type; }

    sf::Vector2f GetSize() const noexcept { return m_size; }

    bool IsDestroyed() const noexcept;
    void Destroy() noexcept;

    //Sprite sprite;

    sf::RectangleShape shadow;

private:
    GameEngine *m_engine;

    sf::Vector2f velocity;

    sf::Vector2f m_size;
    int m_type;

    bool m_isDestroyed;
    bool m_isVisible;

    static const sf::Vector2f SIZE;
    static const int SHADOW_OPACITY;
};

#endif // BRICK_HPP
