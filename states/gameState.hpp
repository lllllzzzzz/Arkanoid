#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "..\stateman\gameEngine.hpp"

class GameEngine;

class GameState
{
public:
    GameState();
    ~GameState();

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual void HandleEvents(GameEngine *game) = 0;
    virtual void Update(GameEngine *game) = 0;
    virtual void Draw(GameEngine *game) = 0;
};

#endif // GAME_STATE_HPP
