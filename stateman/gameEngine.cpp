#include <iostream>
#include "gameEngine.hpp"
#include "..\resourceman\resourceManager.hpp"
#include "..\states\gameState.hpp"

GameEngine::GameEngine(const sf::Vector2f windowSize, const int fps, const std::string& windowTitle) :
    m_window{{windowSize.x, windowSize.y}, windowTitle, sf::Style::Titlebar | sf::Style::Close},
    m_windowSize{windowSize},
    m_isWindowMoving{false},
    m_isRunning{true}
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
    //sf::Event event;
    //while (getWindow().pollEvent(event)) {
    //    if (isWindowMoving()) {
    //        getWindow().setPosition({sf::Mouse::getPosition().x - getWindowSize().x / 2, sf::Mouse::getPosition().y - getWindowSize().y / 2});
    //    } if (event.type == sf::Event::MouseButtonPressed/* && event.MouseButton.Button == sf::Mouse::Left*/) {
    //        moveWindow(true);
    //    } else if (event.type == sf::Event::MouseButtonReleased/* && event.MouseButton.Button == sf::Mouse::Left*/) {
    //        moveWindow(false);
    //    } else if (event.type == sf::Event::Closed) {
    //        Quit();
    //    }
    //}
}

void GameEngine::HandleEvents()
{
    states.top()->HandleEvents(/*this*/);
}

void GameEngine::Update()
{
    states.top()->Update(/*this*/);
}

void GameEngine::Draw()
{
    states.top()->Draw(/*this*/);
}

bool GameEngine::isRunning() const noexcept
{
    return m_isRunning;
}

void GameEngine::Quit() noexcept
{
    m_isRunning = false;
}

bool GameEngine::isWindowMoving() const noexcept
{
    return m_isWindowMoving;
}

void GameEngine::moveWindow(const bool moveWindow) noexcept
{
    m_isWindowMoving = moveWindow;
}

sf::RenderWindow& GameEngine::getWindow()
{
    return m_window;
}

sf::Vector2f GameEngine::getWindowSize() const noexcept
{
    return m_windowSize;
}

void GameEngine::setWindowSize(const sf::Vector2f newWindowSize) noexcept
{
    m_windowSize = newWindowSize;
}
