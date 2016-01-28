#ifndef BALL_HPP
#define BALL_HPP

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

/*#include "circle.hpp"*/

using namespace sf;

// Window constants
const std::string windowTitle{"Arkanoid"};
constexpr int windowWidth{750};
constexpr int windowHeight{450};
constexpr int framesPerSecond{60};

// Ball constants
constexpr float ballRadius{8.0f};
constexpr float ballVelocity{8.0f};
constexpr int initialBallX{windowWidth - 64};
constexpr int initialBallY{windowHeight - 64};

class Ball/* : public Circle*/
{
public:
    Ball(float posX, float posY, int radius);
    ~Ball();

    float x() const noexcept;
    float y() const noexcept;
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;

    void update();
    void setVelocity(int x, int y);
    void setPos(int posX, int posY);

    CircleShape shape;
    Vector2f velocity{-ballVelocity, -ballVelocity};
};

#endif // BALL_HPP
