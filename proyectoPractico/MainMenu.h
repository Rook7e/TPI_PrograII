#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "SaveSystem.h"

enum MenuAction {
    MenuNone,
    MenuNewGame,

    MenuLoadSlot1,
    MenuLoadSlot2,
    MenuLoadSlot3,

    MenuDeleteSlot1,
    MenuDeleteSlot2,
    MenuDeleteSlot3,

    MenuQuit
};

class MainMenu {
private:
    sf::Font font;
    bool fontLoaded;

    sf::RectangleShape newGameButton;
    sf::RectangleShape slotButtons[3];
    sf::RectangleShape quitButton;
    sf::RectangleShape deleteButtons[3];
    sf::Text deleteTexts[3];

    sf::Text titleText;
    sf::Text newGameText;
    sf::Text slotTexts[3];
    sf::Text quitText;

    void layout(sf::RenderWindow& window);
    void setupText(sf::Text& text, int size);
    void centerText(sf::Text& text, sf::Vector2f center);
    void drawBackground(sf::RenderWindow& window);
    void updateHover(sf::RenderWindow& window);

public:
    MainMenu();

    MenuAction handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, const SaveSystem& saves);
};

#endif
