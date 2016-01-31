#include "menuState.hpp"
#include <iostream>

MenuState MenuState::m_MenuState;

MenuState::MenuState()
{
    //std::cout << "MenuState constructor" << std::endl;

    items.emplace_back("Resume", 1);
    //items.emplace_back("Sound: ON", 2);
    items.emplace_back("Exit", 2);

    selectedItem = 1;

    font.loadFromFile("data\\font\\arial.ttf");
    str.setCharacterSize(18);
    //str.setStyle(sf::Text::Bold);
    str.setFont(font);

    for (auto i : items) {
        std::cout << i.text << " : " << i.id << std::endl;
    }
}

MenuState::~MenuState()
{

}

void MenuState::Pause()
{

}

void MenuState::Resume()
{

}

void MenuState::HandleEvents(GameEngine *game)
{
    // Poll window events, required so player can interact with game window
    sf::Event event;
    while (game->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game->window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        //game->popState();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        selectedItem -= (selectedItem > 1);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        selectedItem += (selectedItem < items.size());
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        //items.at(selectedItem - 1).doAction(game);
        if (selectedItem == 1) {
            game->popState();
        } else if (selectedItem == 2) {
            game->Quit();
        }
    }
}

void MenuState::Update(GameEngine *game)
{

}

void MenuState::Draw(GameEngine *game)
{
    game->window.clear(sf::Color::Black);

    for (auto i : items) {
        sf::Text str;
        str.setString(i.text);
        str.setPosition(50, (i.id == 1) ? 30 : 25 * i.id);
        str.setCharacterSize(18);
        //str.setStyle(sf::Text::Bold);
        str.setFont(font);

        //str.setColor(i.isSelected ? sf::Color::Green : sf::Color::White);
        str.setColor((selectedItem == i.id) ? sf::Color::Green : sf::Color::White);

        game->window.draw(str);
    }

    game->window.display();
}

void MenuState::drawMenu()
{

}
