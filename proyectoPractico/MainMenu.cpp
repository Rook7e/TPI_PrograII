#include "MainMenu.h"

MainMenu::MainMenu() {
    fontLoaded = font.loadFromFile("assets/Menu/menu.ttf");

    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    startButton.setSize(sf::Vector2f(180.f, 38.f));
    startButton.setFillColor(sf::Color(75, 88, 105));

    quitButton.setSize(sf::Vector2f(180.f, 38.f));
    quitButton.setFillColor(sf::Color(55, 48, 58));

    if (fontLoaded) {
        setupText(titleText, "Proyecto Practico", 34);
        setupText(startText, "Start Game", 18);
        setupText(quitText, "Quit", 18);
    }
}

void MainMenu::setupText(sf::Text& text, const std::string& value, int size) {
    text.setFont(font);
    text.setString(value);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(235, 235, 230));
}

void MainMenu::centerText(sf::Text& text, sf::Vector2f center) {
    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    text.setPosition(center);
}

void MainMenu::layout(sf::RenderWindow& window) {
    sf::Vector2u size = window.getSize();

    float centerX = size.x / 2.f;
    float centerY = size.y / 2.f;

    startButton.setPosition(centerX - 90.f, centerY - 10.f);
    quitButton.setPosition(centerX - 90.f, centerY + 34.f);

    if (fontLoaded) {
        centerText(titleText, sf::Vector2f(centerX, centerY - 120.f));
        centerText(startText, sf::Vector2f(centerX, centerY + 9.f));
        centerText(quitText, sf::Vector2f(centerX, centerY + 53.f));
    }
}

MenuAction MainMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    layout(window);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            return MenuStart;
        }

        if (event.key.code == sf::Keyboard::Escape) {
            return MenuQuit;
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        if (startButton.getGlobalBounds().contains(mouse)) {
            return MenuStart;
        }

        if (quitButton.getGlobalBounds().contains(mouse)) {
            return MenuQuit;
        }
    }

    return MenuNone;
}

void MainMenu::drawBackground(sf::RenderWindow& window) {
    sf::Vector2u size = window.getSize();

    window.clear(sf::Color(18, 18, 28));

    sf::RectangleShape sky;
    sky.setSize(sf::Vector2f((float)size.x, (float)size.y));
    sky.setFillColor(sf::Color(22, 24, 38));
    sky.setPosition(0.f, 0.f);
    window.draw(sky);

    sf::RectangleShape horizon;
    horizon.setSize(sf::Vector2f((float)size.x, 130.f));
    horizon.setFillColor(sf::Color(48, 38, 58));
    horizon.setPosition(0.f, size.y * 0.48f);
    window.draw(horizon);

    sf::RectangleShape fog;
    fog.setSize(sf::Vector2f((float)size.x, 8.f));
    fog.setFillColor(sf::Color(105, 92, 115));
    fog.setPosition(0.f, size.y * 0.52f);
    window.draw(fog);

    fog.setFillColor(sf::Color(75, 68, 90));
    fog.setPosition(0.f, size.y * 0.58f);
    window.draw(fog);

    sf::CircleShape moon(42.f);
    moon.setFillColor(sf::Color(210, 205, 185));
    moon.setPosition(size.x - 140.f, 55.f);
    window.draw(moon);

    sf::CircleShape shadow(42.f);
    shadow.setFillColor(sf::Color(22, 24, 38));
    shadow.setPosition(size.x - 122.f, 48.f);
    window.draw(shadow);
}

void MainMenu::drawMountain(sf::RenderWindow& window, sf::Vector2f position, float scale) {
    sf::ConvexShape mountain;
    mountain.setPointCount(5);
    mountain.setPoint(0, sf::Vector2f(0.f, 240.f) * scale + position);
    mountain.setPoint(1, sf::Vector2f(130.f, 90.f) * scale + position);
    mountain.setPoint(2, sf::Vector2f(260.f, 240.f) * scale + position);
    mountain.setPoint(3, sf::Vector2f(260.f, 360.f) * scale + position);
    mountain.setPoint(4, sf::Vector2f(0.f, 360.f) * scale + position);
    mountain.setFillColor(sf::Color(42, 51, 66));
    window.draw(mountain);

    sf::ConvexShape snow;
    snow.setPointCount(5);
    snow.setPoint(0, sf::Vector2f(96.f, 130.f) * scale + position);
    snow.setPoint(1, sf::Vector2f(130.f, 90.f) * scale + position);
    snow.setPoint(2, sf::Vector2f(166.f, 132.f) * scale + position);
    snow.setPoint(3, sf::Vector2f(148.f, 124.f) * scale + position);
    snow.setPoint(4, sf::Vector2f(112.f, 128.f) * scale + position);
    snow.setFillColor(sf::Color(190, 195, 205));
    window.draw(snow);
}

void MainMenu::draw(sf::RenderWindow& window) {
    layout(window);
    drawBackground(window);

    sf::Vector2u size = window.getSize();

    drawMountain(window, sf::Vector2f(-40.f, size.y - 390.f), 1.35f);
    drawMountain(window, sf::Vector2f(220.f, size.y - 330.f), 0.95f);
    drawMountain(window, sf::Vector2f(520.f, size.y - 355.f), 1.05f);
    drawMountain(window, sf::Vector2f(690.f, size.y - 305.f), 0.75f);

    window.draw(startButton);
    window.draw(quitButton);

    if (fontLoaded) {
        window.draw(titleText);
        window.draw(startText);
        window.draw(quitText);
    }
}
