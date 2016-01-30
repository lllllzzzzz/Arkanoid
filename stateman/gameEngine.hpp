#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <stack>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "..\states\gameState.hpp"

class GameState;

class GameEngine
{
public:
    GameEngine(const int width, const int height, const int fps, bool fullscreen);
    ~GameEngine();

    void pushState(GameState* state);
    void popState();
    GameState* peekState();
    void changeState(GameState* state);

    void HandleEvents();
    void Update();
    void Draw();

    bool isRunning() const noexcept { return m_isRunning; }
    void Quit() { m_isRunning = false; }

    std::stack<GameState*> states;
    sf::RenderWindow window;

    int windowWidth;
    int windowHeight;

private:
    bool m_isRunning;
};

#endif // GAME_ENGINE_HPP
