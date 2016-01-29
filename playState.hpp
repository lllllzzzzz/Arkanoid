#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include <SFML/Audio.hpp>
#include "gameState.hpp"

class PlayState : public GameState
{
public:
    PlayState();
    ~PlayState();

    void Pause();
    void Resume();

    void HandleEvents();
    void Update();
    void Draw();

private:
};

#endif // PLAY_STATE_HPP
