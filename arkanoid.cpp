#include <cassert>

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "game.hpp"
#include "ball.hpp"
#include "paddle.hpp"
#include "brick.hpp"

constexpr int framesRate{60};
constexpr int numLives{2};

using namespace sf;

int main()
{
    Game game{framesRate, numLives};
    game.newGame();
    game.runGame();
    return 0;
}
