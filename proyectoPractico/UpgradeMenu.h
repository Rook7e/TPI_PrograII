#ifndef UPGRADEMENU_H_INCLUDED
#define UPGRADEMENU_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "Progression.h"

// Acciones posibles del menu de mejoras.
enum UpgradeAction {
    UpgradeNone,
    UpgradeBuyDamage,
    UpgradeBuyRange,
    UpgradeBuyCapacity,
    UpgradeContinue
};

// Menu que aparece entre pisos.
// Permite gastar oro en mejoras de la aspiradora.
class UpgradeMenu {
private:
    sf::Font font;
    bool fontLoaded;

    // Botones visuales.
    sf::RectangleShape damageButton;
    sf::RectangleShape rangeButton;
    sf::RectangleShape continueButton;
    sf::RectangleShape capacityButton;

    // Textos del menu.
    sf::Text titleText;
    sf::Text infoText;
    sf::Text damageText;
    sf::Text rangeText;
    sf::Text continueText;
    sf::Text capacityText;

    // Configura texto con fuente/tamano/color.
    void setupText(sf::Text& text, int size);

    // Centra texto en una posicion.
    void centerText(sf::Text& text, sf::Vector2f center);

    // Calcula posiciones y actualiza textos segun Progression.
    void layout(sf::RenderWindow& window, Progression& progression);

public:
    UpgradeMenu();

    // Procesa clicks y devuelve compra/continuar.
    UpgradeAction handleEvent(sf::Event& event, sf::RenderWindow& window);

    // Dibuja menu con oro, score, basura y costos.
    void draw(sf::RenderWindow& window, Progression& progression);
};

#endif
