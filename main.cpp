#include "stateman\gameEngine.hpp"
#include "states\playState.hpp"

const std::string WINDOW_TITLE = "Arkanoid";
constexpr int WINDOW_WIDTH{480};
constexpr int WINDOW_HEIGHT{620};
constexpr int FRAMES_PER_SECOND{60};

//using namespace Arkanoid;

int main()
{
    /*Arkanoid::*/GameEngine game{{WINDOW_WIDTH, WINDOW_HEIGHT}, FRAMES_PER_SECOND, WINDOW_TITLE};

    // Enter Play Game state at startup
    game.changeState(/*Arkanoid::*/PlayState::Instance());
    //game.changeState(/*Arkanoid::*/new PlayState());

    // Game engine loop
    while (game.isRunning()) {
        game.Run();
    }

    return 0;
}
