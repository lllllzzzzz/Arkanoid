#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

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

// Paddle constants
constexpr int paddleWidth{80};
constexpr int paddleHeight{15};
constexpr int paddleVelocity{10.0f};
constexpr int initialPaddleX{windowWidth / 2};
constexpr int initialPaddleY{windowHeight - 25};

// Brick constants
constexpr int brickWidth{55};
constexpr int brickHeight{20};
constexpr int countBricksX{11};
constexpr int countBricksY{4};

// Game constants
constexpr int numLivesDefault{2};

struct Circle
{
    CircleShape shape;
    float x() const noexcept        { return shape.getPosition().x; }
    float y() const noexcept        { return shape.getPosition().y; }
    float left() const noexcept     { return x() - shape.getRadius(); }
    float right() const noexcept    { return x() + shape.getRadius(); }
    float top() const noexcept      { return y() - shape.getRadius(); }
    float bottom() const noexcept   { return y() + shape.getRadius(); }
    void setPos(int posX, int posY) { shape.setPosition(posX, posY); }
};

struct Rectangle
{
    RectangleShape shape;
    float x() const noexcept        { return shape.getPosition().x; }
    float y() const noexcept        { return shape.getPosition().y; }
    float left() const noexcept     { return x() - shape.getSize().x / 2.f; }
    float right() const noexcept    { return x() + shape.getSize().x / 2.f; }
    float top() const noexcept      { return y() - shape.getSize().y / 2.f; }
    float bottom() const noexcept   { return y() + shape.getSize().y / 2.f; }
    void setPos(int posX, int posY) { shape.setPosition(posX, posY); }
};

struct Ball : public Circle
{
    Vector2f velocity{-ballVelocity, -ballVelocity};

    Ball(float posX, float posY)
    {
        shape.setPosition(posX, posY);
        shape.setRadius(ballRadius);
        shape.setFillColor(Color::White);
        shape.setOrigin(ballRadius, ballRadius);
    }

    void update()
    {
        shape.move(velocity);

        // Keep the ball in horizontal bounds of the window
        if (left() < 0) {
            velocity.x = ballVelocity;
        } else if (right() > windowWidth) {
            velocity.x = -ballVelocity;
        }
        // Keep the ball in vertical bounds of the window
        if (top() < 0) {
            velocity.y = ballVelocity;
        } else if (bottom() > windowHeight) {
            velocity.y = -ballVelocity;
        }
    }
};

struct Paddle : public Rectangle
{
    Vector2f velocity;

    Paddle(float posX, float posY)
    {
        shape.setPosition(posX, posY);
        shape.setSize({paddleWidth, paddleHeight});
        shape.setFillColor(Color::White);
        shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
    }

    void update()
    {
        shape.move(velocity);

        // Handle player input (move paddle left or right)
        if (Keyboard::isKeyPressed(Keyboard::Key::A) && left() > 0) {
            velocity.x = -paddleVelocity;
        } else if (Keyboard::isKeyPressed(Keyboard::Key::D)
            && right() < windowWidth) {
            velocity.x = paddleVelocity;
        } else {
            velocity.x = 0;
        }
    }
};

struct Brick : public Rectangle
{
    bool destroyed{false};

    Brick(float posX, float posY)
    {
        shape.setPosition(posX, posY);
        shape.setSize({brickWidth, brickHeight});
        shape.setFillColor(Color::Red);
        shape.setOrigin(brickWidth / 2.f, brickHeight / 2.f);
    }
};

// Check if two shapes are intersecting/colliding
// Use this function for collision detection in the game
template <class T1, class T2>
bool isIntersecting(T1& mA, T2& mB)
{
    return mA.right() >= mB.left() && mA.left() <= mB.right()
        && mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}

void testCollision(Paddle& mPaddle, Ball& mBall)
{
    if (!isIntersecting(mPaddle, mBall)) {
        return;
    }

    // Ball has hit paddle, ball now bounces off paddle and moves upwards
    mBall.velocity.y = -ballVelocity;
    // Horizontal direction of ball depends on where the ball hit the paddle
    mBall.velocity.x = (mBall.x() < mPaddle.x()) ? -ballVelocity : ballVelocity;
}

void testCollision(Brick& mBrick, Ball& mBall)
{
    if (!isIntersecting(mBrick, mBall)) {
        return;
    }

    mBrick.destroyed = true;

    // Calculate intersections of ball/brick
    float overlapLeft{mBall.right() - mBrick.left()};
    float overlapRight{mBrick.right() - mBall.left()};
    float overlapTop{mBall.bottom() - mBrick.top()};
    float overlapBottom{mBrick.bottom() - mBall.top()};

    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    float minOverlapX{ballFromLeft ? overlapLeft : overlapRight};
    float minOverlapY{ballFromTop ? overlapTop : overlapBottom};

    if (abs(minOverlapX) < abs(minOverlapY)) {
        mBall.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
    } else {
        mBall.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
    }
}

struct Game
{
    RenderWindow window{{windowWidth, windowHeight}, windowTitle};
    Ball ball{initialBallX, initialBallY};
    Paddle paddle{initialPaddleX, initialPaddleY};
    std::vector<Brick> bricks;
    Text textLives;
    int numLives{numLivesDefault};

    Game()
    {
        window.setFramerateLimit(framesPerSecond);

        for (int iX{0}; iX < countBricksX; iX++) {
            for (int iY{0}; iY < countBricksY; iY++) {
                bricks.emplace_back((iX + 1) * (brickWidth + 3) + 22,
                    (iY + 2) * (brickHeight + 3));
            }
        }
    }

    void run()
    {
        while (!Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            if (numLives == 0) {
                reset();
            }

            if ((ball.y() == windowHeight)) {
                numLives--;
                ball.setPos(initialBallX, initialBallY);
                paddle.setPos(initialPaddleX, initialPaddleY);
                //sleep(milliseconds(500));
            }

            window.clear(Color::Black);

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                    break;
                }
            }

            ball.update();
            paddle.update();

            testCollision(paddle, ball);

            for (auto& brick : bricks) {
                testCollision(brick, ball);
            }

            // Lambda function to remove destroyed blocks from block vector
            bricks.erase(remove_if(begin(bricks), end(bricks),
                [](const Brick& mBrick) { return mBrick.destroyed; }), end(bricks));

            window.draw(ball.shape);
            window.draw(paddle.shape);
            for (auto& brick : bricks) {
                window.draw(brick.shape);
            }

            window.display();
        }
    }

    void reset()
    {
        for (int iX{0}; iX < countBricksX; iX++) {
            for (int iY{0}; iY < countBricksY; iY++) {
                bricks.emplace_back((iX + 1) * (brickWidth + 3) + 22,
                    (iY + 2) * (brickHeight + 3));
            }
        }

        ball.setPos(initialBallX, initialBallY);
        paddle.setPos(initialPaddleX, initialPaddleY);

        numLives = numLivesDefault;
    }
};

int main()
{
    Game{}.run();
    return 0;
}
