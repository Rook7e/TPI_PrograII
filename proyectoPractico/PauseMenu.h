#ifndef PAUSEMENU_H_INCLUDED
#define PAUSEMENU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "SaveSystem.h"

enum PauseAction {
    PauseNone,
    PauseResume,
    PauseSave,
    PauseLoad,
    PauseMainMenu,
    PauseQuit
};

class PauseMenu {
private:
    sf::Font font;
    bool fontLoaded;
    int selectedSlot;

    sf::Text titleText;
    sf::Text slotText;
    sf::Text optionsText;
    sf::RectangleShape panel;

    void centerText(sf::Text& text, sf::Vector2f center);

public:
    PauseMenu();

    PauseAction handleEvent(sf::Event& event);
    void draw(sf::RenderWindow& window, const SaveSystem& saves);
    int getSelectedSlot() const;
    void setSelectedSlot(int slot);
};

#endif
