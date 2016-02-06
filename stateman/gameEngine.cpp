#include <iostream>
#include "gameEngine.hpp"
#include "..\resourceman\resourceManager.hpp"
#include "..\states\gameState.hpp"

//using namespace Arkanoid;

GameEngine::GameEngine(const int width, const int height, const int fps, const std::string& windowTitle) :
    m_window({width, height}, windowTitle, sf::Style::Titlebar | sf::Style::Close),
    m_windowWidth(width),
    m_windowHeight(height),
    m_isWindowMoving(false)
{
    m_window.setFramerateLimit(fps);
}

GameEngine::~GameEngine()
{

}

void GameEngine::pushState(GameState* state)
{
    if (!states.empty()) {
        states.top()->Pause();
    }

    states.push(state);

    peekState()->Init(this);
}

void GameEngine::popState()
{
    delete states.top();
    states.pop();
}

void GameEngine::changeState(GameState* state)
{
    if (!states.empty()) {
        popState();
    }

    pushState(state);

    peekState()->Init(this);
}

GameState* GameEngine::peekState()
{
    if (states.empty()) {
        return nullptr;
    }

    return states.top();
}

void GameEngine::Run()
{
    HandleWindowEvents();
    HandleEvents();
    Update();
    Draw();
}

void GameEngine::HandleWindowEvents()
{
    // Poll window events
    sf::Event event;
    while (getWindow().pollEvent(event)) {
        if (isWindowMoving()) {
            std::cout << "left mouse button clicked\n";
            getWindow().setPosition({sf::Mouse::getPosition().x - getWindowWidth() / 2, sf::Mouse::getPosition().y - getWindowHeight() / 2});
        } if (event.type == sf::Event::MouseButtonPressed/* && event.MouseButton.Button == sf::Mouse::Left*/) {
            moveWindow(true);
        } if (event.type == sf::Event::MouseButtonReleased/* && event.MouseButton.Button == sf::Mouse::Left*/) {
            moveWindow(false);
            Quit();
        }
    }
}

void GameEngine::HandleEvents()
{
    states.top()->HandleEvents(this);
}

void GameEngine::Update()
{
    states.top()->Update(this);
}

void GameEngine::Draw()
{
    states.top()->Draw(this);
}

bool GameEngine::isRunning() const noexcept
{
    return m_isRunning;
}

void GameEngine::Quit()
{
    m_isRunning = false;
}

bool GameEngine::isWindowMoving() const noexcept
{
    return m_isWindowMoving;
}

void GameEngine::moveWindow(const bool moveWindow)
{
    m_isWindowMoving = moveWindow;
}

sf::RenderWindow& GameEngine::getWindow()
{
    return m_window;
}

int GameEngine::getWindowWidth() const noexcept
{
    return m_windowWidth;
}

int GameEngine::getWindowHeight() const noexcept
{
    return m_windowHeight;
}

void GameEngine::setWindowSize(const int width, const int height)
{
    m_windowWidth = width;
    m_windowWidth = height;
}
