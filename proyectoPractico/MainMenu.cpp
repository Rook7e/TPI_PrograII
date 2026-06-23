#include "MainMenu.h"

namespace {
const sf::Color BUTTON_COLOR(48, 49, 62);
const sf::Color BUTTON_HOVER(76, 68, 92);
const sf::Color BUTTON_ACCENT(72, 86, 92);
}

MainMenu::MainMenu() {
    fontLoaded = font.loadFromFile("assets/Menu/menu.ttf");
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    newGameButton.setSize(sf::Vector2f(500.f, 48.f));
    quitButton.setSize(sf::Vector2f(500.f, 48.f));

    for (int i = 0; i < 3; i++) {
    slotButtons[i].setSize(sf::Vector2f(390.f, 48.f));
    slotButtons[i].setFillColor(BUTTON_COLOR);

    deleteButtons[i].setSize(sf::Vector2f(100.f, 48.f));
    deleteButtons[i].setFillColor(sf::Color(90, 42, 48));

    if (fontLoaded) {
        setupText(deleteTexts[i], 17);
        deleteTexts[i].setString("Eliminar");
    }
}

    newGameButton.setFillColor(BUTTON_ACCENT);
    quitButton.setFillColor(sf::Color(65, 43, 50));

    if (fontLoaded) {
        setupText(titleText, 40);
        setupText(newGameText, 21);
        setupText(quitText, 21);

        for (int i = 0; i < 3; i++) {
            setupText(slotTexts[i], 18);
        }
    }
}

void MainMenu::setupText(sf::Text& text, int size) {
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(235, 235, 230));
}

void MainMenu::centerText(sf::Text& text, sf::Vector2f center) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f,
                   bounds.top + bounds.height / 2.f);
    text.setPosition(center);
}

void MainMenu::layout(sf::RenderWindow& window) {
    sf::Vector2u size = window.getSize();

    float centerX = size.x / 2.f;
    float firstY = size.y / 2.f - 100.f;

    newGameButton.setPosition(centerX - 250.f, firstY);

    for (int i = 0; i < 3; i++) {
        float y = firstY + 62.f * (i + 1);

        slotButtons[i].setPosition(centerX - 250.f, y);
        deleteButtons[i].setPosition(centerX + 150.f, y);

        if (fontLoaded) {
            centerText(
                slotTexts[i],
                sf::Vector2f(centerX - 55.f, y + 24.f)
            );

            centerText(
                deleteTexts[i],
                sf::Vector2f(centerX + 200.f, y + 24.f)
            );
        }
    }

    quitButton.setPosition(centerX - 250.f, firstY + 248.f);

    if (!fontLoaded) {
        return;
    }

    centerText(titleText, sf::Vector2f(centerX, firstY - 100.f));
    centerText(newGameText, sf::Vector2f(centerX, firstY + 24.f));
    centerText(quitText, sf::Vector2f(centerX, firstY + 272.f));
}

void MainMenu::updateHover(sf::RenderWindow& window) {
    sf::Vector2i pixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouse = window.mapPixelToCoords(pixel);

    newGameButton.setFillColor(
        newGameButton.getGlobalBounds().contains(mouse)
            ? BUTTON_HOVER : BUTTON_ACCENT);

    for (int i = 0; i < 3; i++) {
        slotButtons[i].setFillColor(
            slotButtons[i].getGlobalBounds().contains(mouse)
                ? BUTTON_HOVER : BUTTON_COLOR);
    }

    quitButton.setFillColor(
        quitButton.getGlobalBounds().contains(mouse)
            ? sf::Color(100, 50, 58) : sf::Color(65, 43, 50));
}

MenuAction MainMenu::handleEvent(
    sf::Event& event,
    sf::RenderWindow& window
) {
    layout(window);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Num1) {
            return MenuLoadSlot1;
        }

        if (event.key.code == sf::Keyboard::Num2) {
            return MenuLoadSlot2;
        }

        if (event.key.code == sf::Keyboard::Num3) {
            return MenuLoadSlot3;
        }

        if (event.key.code == sf::Keyboard::Escape) {
            return MenuQuit;
        }
    }

    if (event.type != sf::Event::MouseButtonPressed ||
        event.mouseButton.button != sf::Mouse::Left) {
        return MenuNone;
    }

    sf::Vector2f mouse = window.mapPixelToCoords(
        sf::Vector2i(
            event.mouseButton.x,
            event.mouseButton.y
        )
    );

    if (slotButtons[0].getGlobalBounds().contains(mouse)) {
        return MenuLoadSlot1;
    }

    if (slotButtons[1].getGlobalBounds().contains(mouse)) {
        return MenuLoadSlot2;
    }

    if (slotButtons[2].getGlobalBounds().contains(mouse)) {
        return MenuLoadSlot3;
    }

    if (deleteButtons[0].getGlobalBounds().contains(mouse)) {
        return MenuDeleteSlot1;
    }

    if (deleteButtons[1].getGlobalBounds().contains(mouse)) {
        return MenuDeleteSlot2;
    }

    if (deleteButtons[2].getGlobalBounds().contains(mouse)) {
        return MenuDeleteSlot3;
    }

    if (quitButton.getGlobalBounds().contains(mouse)) {
        return MenuQuit;
    }

    return MenuNone;
}

void MainMenu::drawBackground(sf::RenderWindow& window) {
    sf::Vector2u size = window.getSize();
    window.clear(sf::Color(12, 12, 18));

    sf::RectangleShape haze(sf::Vector2f((float)size.x, size.y * 0.45f));
    haze.setPosition(0.f, size.y * 0.55f);
    haze.setFillColor(sf::Color(28, 27, 38));
    window.draw(haze);

    sf::CircleShape moon(54.f);
    moon.setPosition(size.x - 180.f, 75.f);
    moon.setFillColor(sf::Color(185, 183, 165));
    window.draw(moon);

    sf::CircleShape shadow(55.f);
    shadow.setPosition(size.x - 155.f, 62.f);
    shadow.setFillColor(sf::Color(12, 12, 18));
    window.draw(shadow);
}

void MainMenu::draw(
    sf::RenderWindow& window,
    const SaveSystem& saves
) {
    drawBackground(window);

    titleText.setString("Aprentice`s Mess");
    newGameText.setString("Elegi una ranura para comenzar");
    quitText.setString("Salir [Escapar]");

    for (int i = 0; i < 3; i++) {
        slotTexts[i].setString(saves.getSummary(i + 1));
        deleteTexts[i].setString("Eliminar");
    }

    layout(window);
    updateHover(window);

    window.draw(newGameButton);

    for (int i = 0; i < 3; i++) {
        window.draw(slotButtons[i]);
        window.draw(deleteButtons[i]);
    }

    window.draw(quitButton);

    if (!fontLoaded) {
        return;
    }

    window.draw(titleText);
    window.draw(newGameText);

    for (int i = 0; i < 3; i++) {
        window.draw(slotTexts[i]);
        window.draw(deleteTexts[i]);
    }

    window.draw(quitText);
}
