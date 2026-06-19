#include "PauseMenu.h"

PauseMenu::PauseMenu() {
    fontLoaded = font.loadFromFile("assets/Menu/menu.ttf");
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    selectedSlot = 1;
    panel.setSize(sf::Vector2f(620.f, 430.f));
    panel.setFillColor(sf::Color(18, 18, 28, 245));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(100, 90, 120));

    if (fontLoaded) {
        titleText.setFont(font);
        titleText.setCharacterSize(36);
        titleText.setFillColor(sf::Color(235, 235, 230));

        slotText.setFont(font);
        slotText.setCharacterSize(20);
        slotText.setFillColor(sf::Color(205, 190, 130));

        optionsText.setFont(font);
        optionsText.setCharacterSize(22);
        optionsText.setFillColor(sf::Color(220, 220, 220));
    }
}

void PauseMenu::centerText(sf::Text& text, sf::Vector2f center) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f,
                   bounds.top + bounds.height / 2.f);
    text.setPosition(center);
}

PauseAction PauseMenu::handleEvent(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) {
        return PauseNone;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        return PauseResume;
    }

    // Números de arriba y teclado numérico
    if (event.key.code == sf::Keyboard::Num1 ||
        event.key.code == sf::Keyboard::Numpad1) {
        selectedSlot = 1;
        return PauseNone;
    }

    if (event.key.code == sf::Keyboard::Num2 ||
        event.key.code == sf::Keyboard::Numpad2) {
        selectedSlot = 2;
        return PauseNone;
    }

    if (event.key.code == sf::Keyboard::Num3 ||
        event.key.code == sf::Keyboard::Numpad3) {
        selectedSlot = 3;
        return PauseNone;
    }

    if (event.key.code == sf::Keyboard::S) {
        return PauseSave;
    }

    if (event.key.code == sf::Keyboard::L) {
        return PauseLoad;
    }

    if (event.key.code == sf::Keyboard::M) {
        return PauseMainMenu;
    }

    if (event.key.code == sf::Keyboard::Q) {
        return PauseQuit;
    }

    return PauseNone;
}

void PauseMenu::draw(sf::RenderWindow& window, const SaveSystem& saves) {
    sf::Vector2u size = window.getSize();
    const float centerX = size.x / 2.f;
    const float centerY = size.y / 2.f;

    sf::RectangleShape overlay(sf::Vector2f((float)size.x, (float)size.y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    panel.setPosition(centerX - 310.f, centerY - 215.f);
    window.draw(panel);

    if (!fontLoaded) return;

        slotText.setString(
        "Ranura seleccionada: " +
        std::to_string(selectedSlot) +
        "\n" +
        saves.getSummary(selectedSlot)
    );

    optionsText.setString(
        "1 / 2 / 3  Elegir ranura\n\n"
        "S  Guardar en la ranura seleccionada\n"
        "L  Cargar la ranura seleccionada\n"
        "Escape  Continuar\n"
        "M  Menu principal\n"
        "Q  Salir del juego"
    );

    centerText(titleText, sf::Vector2f(centerX, centerY - 155.f));
    centerText(slotText, sf::Vector2f(centerX, centerY - 95.f));
    centerText(optionsText, sf::Vector2f(centerX, centerY + 45.f));

    window.draw(titleText);
    window.draw(slotText);
    window.draw(optionsText);
}

int PauseMenu::getSelectedSlot() const {
    return selectedSlot;
}
void PauseMenu::setSelectedSlot(int slot) {
    if (slot >= 1 && slot <= 3) {
        selectedSlot = slot;
    }
}
