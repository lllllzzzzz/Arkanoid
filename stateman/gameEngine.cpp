#include <iostream>
#include "gameEngine.hpp"
#include "..\states\gameState.hpp"

GameEngine::GameEngine(const int width, const int height, const int fps, bool fullscreen) :
window(sf::VideoMode(width, height), "Arkanoid", fullscreen ? sf::Style::Fullscreen : sf::Style::Default)
{
    this->windowWidth = width;
    this->windowHeight = height;
    window.setFramerateLimit(fps);
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
}

void GameEngine::popState()
{
    delete states.top();
    states.pop();
}

GameState* GameEngine::peekState()
{
    if (states.empty()) {
        return nullptr;
    }

    return states.top();
}

void GameEngine::changeState(GameState* state)
{
    if (!states.empty()) {
        popState();
    }

    pushState(state);
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
