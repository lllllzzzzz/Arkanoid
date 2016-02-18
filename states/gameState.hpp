#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//#include "..\stateman\gameEngine.hpp"

class GameEngine;

//namespace Arkanoid
//{
    class GameState
    {
    public:
        GameState();
        ~GameState();

        virtual void Init(GameEngine *game);
        virtual void Cleanup();

        virtual void Pause() = 0;
        virtual void Resume() = 0;

        virtual void HandleEvents() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;
        virtual void LoadResources() = 0;
        virtual void LoadObjects() = 0;
    };
//}

#endif // GAME_STATE_HPP
