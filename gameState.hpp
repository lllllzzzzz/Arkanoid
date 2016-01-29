#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Audio.hpp>

class GameState
{
public:
    GameState();
    ~GameState();

    void Pause();
    void Resume();

    void HandleEvents();
    void Update();
    void Draw();

private:
};

#endif // GAME_STATE_HPP
