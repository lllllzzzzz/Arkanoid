#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <stack>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "..\resourceman\resourceManager.hpp"
//#include "..\states\gameState.hpp"

class GameState;

/*namespace Arkanoid
{*/
    class GameEngine
    {
    public:
        GameEngine(const int width, const int height, const int fps, const std::string& windowTitle);
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
        void Quit();

        bool isWindowMoving() const noexcept;
        void moveWindow(const bool moveWindow);

        sf::RenderWindow& getWindow();
        int getWindowWidth() const noexcept;
        int getWindowHeight() const noexcept;
        void setWindowSize(const int width, const int height);

        ResourceManager resourceMan;

    private:
        sf::RenderWindow m_window;

        std::stack<GameState*> states;

        bool m_isRunning;
        bool m_isWindowMoving;
        int m_windowWidth;
        int m_windowHeight;
    };
//}

#endif // GAME_ENGINE_HPP
