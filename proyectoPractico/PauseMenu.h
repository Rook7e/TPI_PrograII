#ifndef PAUSEMENU_H_INCLUDED
#define PAUSEMENU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "SaveSystem.h"

// Acciones posibles desde pausa.
enum PauseAction {
    PauseNone,
    PauseResume,
    PauseSave,
    PauseLoad,
    PauseMainMenu,
    PauseQuit
};

// Menu de pausa.
// Permite guardar/cargar, elegir ranura, volver al menu o salir.
class PauseMenu {
private:
    sf::Font font;
    bool fontLoaded;

    // Ranura seleccionada para guardar o cargar.
    int selectedSlot;

    // Textos del menu.
    sf::Text titleText;
    sf::Text slotText;
    sf::Text optionsText;

    // Panel oscuro de fondo.
    sf::RectangleShape panel;

    // Helper para centrar textos.
    void centerText(sf::Text& text, sf::Vector2f center);

public:
    PauseMenu();

    // Procesa teclas y devuelve una accion.
    PauseAction handleEvent(sf::Event& event);

    // Dibuja el menu de pausa y resumen de ranura.
    void draw(sf::RenderWindow& window, const SaveSystem& saves);

    // Ranura actual.
    int getSelectedSlot() const;
    void setSelectedSlot(int slot);
};

#endif
