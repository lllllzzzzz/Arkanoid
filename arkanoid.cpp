#include <cassert>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "stateman\gameEngine.hpp"
#include "states\playState.hpp"
#include "states\menuState.hpp"

constexpr int windowWidth{770};
constexpr int windowHeight{500};
constexpr int framesRate{60};
constexpr int numLives{2};

int main()
{
    GameEngine game{windowWidth, windowHeight, framesRate, false};

    // Enter Play Game state at startup
    game.changeState(PlayState::Instance());

    // Game engine loop
    while (game.isRunning()) {
        game.HandleEvents();
        game.Update();
        game.Draw();
    }

    return 0;
}
