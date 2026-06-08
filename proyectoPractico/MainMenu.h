#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum MenuAction {
    MenuNone,
    MenuStart,
    MenuQuit
};

class MainMenu {
private:
    sf::Font font;
    bool fontLoaded;

    sf::RectangleShape startButton;
    sf::RectangleShape quitButton;

    sf::Text titleText;
    sf::Text startText;
    sf::Text quitText;

    void layout(sf::RenderWindow& window);
    void setupText(sf::Text& text, const std::string& value, int size);
    void centerText(sf::Text& text, sf::Vector2f center);
    void drawBackground(sf::RenderWindow& window);
    void drawMountain(sf::RenderWindow& window, sf::Vector2f position, float scale);

public:
    MainMenu();

    MenuAction handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
};

#endif
