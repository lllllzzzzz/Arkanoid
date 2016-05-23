#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <stack>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "..\resourceman\resourceManager.hpp"

class GameState;

/*namespace Arkanoid
{*/
    class GameEngine
    {
    public:
        GameEngine(const sf::Vector2f windowSize, const int fps, const std::string& windowTitle);
        ~GameEngine();

        void pushState(GameState* state);
        void popState();
        void changeState(GameState* state);
        GameState* peekState();

        void Run();

        void HandleWindowEvents();
        void HandleEvents();
        void Update();
        void Draw();

        bool isRunning() const noexcept;
        void Quit() noexcept;

        bool isWindowMoving() const noexcept;
        void moveWindow(const bool moveWindow) noexcept;

        sf::RenderWindow& getWindow();
        sf::Vector2f getWindowSize() const noexcept;
        void setWindowSize(const sf::Vector2f newWindowSize) noexcept;

        ResourceManager resourceMan;

    private:
        //static GameEngine m_GameEngine;
        sf::RenderWindow m_window;

        std::stack<GameState*> states;

        sf::Vector2f m_windowSize;
        bool m_isWindowMoving;
        bool m_isRunning;
    };
//}

#endif // GAME_ENGINE_HPP
