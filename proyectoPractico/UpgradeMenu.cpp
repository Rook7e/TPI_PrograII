#include "UpgradeMenu.h"
#include <string>

UpgradeMenu::UpgradeMenu() {
    fontLoaded = font.loadFromFile("assets/menu.ttf");

    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    damageButton.setSize(sf::Vector2f(360.f, 44.f));
    damageButton.setFillColor(sf::Color(70, 45, 85));

    rangeButton.setSize(sf::Vector2f(360.f, 44.f));
    rangeButton.setFillColor(sf::Color(55, 70, 90));

    continueButton.setSize(sf::Vector2f(220.f, 42.f));
    continueButton.setFillColor(sf::Color(80, 90, 65));

    if (fontLoaded) {
        setupText(titleText, 34);
        setupText(infoText, 18);
        setupText(damageText, 17);
        setupText(rangeText, 17);
        setupText(continueText, 18);
        setupText(capacityText, 22);
    }
}

void UpgradeMenu::setupText(sf::Text& text, int size) {
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(235, 235, 230));
}

void UpgradeMenu::centerText(sf::Text& text, sf::Vector2f center) {
    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    text.setPosition(center);
}

void UpgradeMenu::layout(sf::RenderWindow& window, Progression& progression) {
    sf::Vector2u size = window.getSize();

    float centerX = size.x / 2.f;
    float centerY = size.y / 2.f;

    damageButton.setPosition(centerX - 180.f, centerY - 30.f);
    rangeButton.setPosition(centerX - 180.f, centerY + 25.f);
    continueButton.setPosition(centerX - 110.f, centerY + 105.f);

    if (fontLoaded) {
        titleText.setString("Mejoras");
        infoText.setString(
            "Score: " + std::to_string(progression.getScore()) +
            "     Oro: " + std::to_string(progression.getGold()) +
            "     Basura: " + std::to_string((int)progression.getTrashStored()) +
            "/" + std::to_string((int)progression.getTrashCapacity())
        );

                damageText.setString(
            "1  Dano aspiradora    Nv " +
            std::to_string(progression.getVacuumDamageLevel()) +
            "    $" + std::to_string(progression.getVacuumDamageCost())
        );

        rangeText.setString(
            "2  Rango manguera     Nv " +
            std::to_string(progression.getVacuumRangeLevel()) +
            "    $" + std::to_string(progression.getVacuumRangeCost())
        );

        capacityText.setString(
            "3  Capacidad basura   Nv " +
            std::to_string(progression.getVacuumCapacityLevel()) +
            "    $" + std::to_string(progression.getVacuumCapacityCost())
        );

        continueText.setString("Enter - Siguiente piso");

        centerText(titleText, sf::Vector2f(centerX, centerY - 170.f));
        centerText(infoText, sf::Vector2f(centerX, centerY - 120.f));

        damageButton.setPosition(centerX - 210.f, centerY - 55.f);
        rangeButton.setPosition(centerX - 210.f, centerY + 5.f);
        capacityButton.setPosition(centerX - 210.f, centerY + 65.f);
        continueButton.setPosition(centerX - 180.f, centerY + 145.f);

        centerText(damageText, sf::Vector2f(centerX, centerY - 38.f));
        centerText(rangeText, sf::Vector2f(centerX, centerY + 22.f));
        centerText(capacityText, sf::Vector2f(centerX, centerY + 82.f));
        centerText(continueText, sf::Vector2f(centerX, centerY + 165.f));
    }
}

UpgradeAction UpgradeMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1) {
            return UpgradeBuyDamage;
        }

        if (event.key.code == sf::Keyboard::Num2) {
            return UpgradeBuyRange;
        }

        if (event.key.code == sf::Keyboard::Num3) {
            return UpgradeBuyCapacity;
        }

        if (event.key.code == sf::Keyboard::Enter) {
            return UpgradeContinue;
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        if (damageButton.getGlobalBounds().contains(mouse)) {
            return UpgradeBuyDamage;
        }

        if (rangeButton.getGlobalBounds().contains(mouse)) {
            return UpgradeBuyRange;
        }

        if (continueButton.getGlobalBounds().contains(mouse)) {
            return UpgradeContinue;
        }
    }

    return UpgradeNone;
}

void UpgradeMenu::draw(sf::RenderWindow& window, Progression& progression) {
    layout(window, progression);

    window.clear(sf::Color(16, 14, 22));

    sf::RectangleShape panel;
    panel.setSize(sf::Vector2f(520.f, 340.f));
    panel.setOrigin(260.f, 170.f);
    panel.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    panel.setFillColor(sf::Color(28, 26, 38));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(95, 85, 110));
    window.draw(panel);

    window.draw(damageButton);
    window.draw(rangeButton);
    window.draw(capacityText);
    window.draw(capacityButton);
    window.draw(continueButton);

    window.draw(titleText);
    window.draw(infoText);
    window.draw(damageText);
    window.draw(rangeText);

window.draw(continueText);

    if (fontLoaded) {
        window.draw(titleText);
        window.draw(infoText);
        window.draw(damageText);
        window.draw(rangeText);
        window.draw(continueText);
    }
}
