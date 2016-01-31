#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <vector>
#include "gameState.hpp"
#include "..\menu\menuItem.hpp"

class MenuState : public GameState
{
public:
    MenuState();
    ~MenuState();

    void Pause();
    void Resume();

    void HandleEvents(GameEngine *game);
    void Update(GameEngine *game);
    void Draw(GameEngine *game);

    void drawMenu();

    static MenuState* Instance() {
        return &m_MenuState;
    }

private:
    static MenuState m_MenuState;
    std::vector<MenuItem> items;
    int selectedItem;
    sf::Text str;
    sf::Font font;
};

#endif // MENU_STATE_HPP
