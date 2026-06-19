#ifndef UPGRADEMENU_H_INCLUDED
#define UPGRADEMENU_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "Progression.h"

enum UpgradeAction {
    UpgradeNone,
    UpgradeBuyDamage,
    UpgradeBuyRange,
    UpgradeBuyCapacity,
    UpgradeContinue
};

class UpgradeMenu {
private:
    sf::Font font;
    bool fontLoaded;

    sf::RectangleShape damageButton;
    sf::RectangleShape rangeButton;
    sf::RectangleShape continueButton;
    sf::RectangleShape capacityButton;


    sf::Text titleText;
    sf::Text infoText;
    sf::Text damageText;
    sf::Text rangeText;
    sf::Text continueText;
    sf::Text capacityText;

    void setupText(sf::Text& text, int size);
    void centerText(sf::Text& text, sf::Vector2f center);
    void layout(sf::RenderWindow& window, Progression& progression);



public:
    UpgradeMenu();

    UpgradeAction handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, Progression& progression);
};

#endif
