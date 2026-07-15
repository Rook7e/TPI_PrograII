#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "SaveSystem.h"

// Acciones posibles del menu principal.
// El Game interpreta estas acciones y decide que hacer.
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

// Menu principal.
// Muestra slots de guardado, opcion de nueva partida, borrar y salir.
class MainMenu {
private:
    sf::Font font;
    bool fontLoaded;

    // Botones principales.
    sf::RectangleShape newGameButton;
    sf::RectangleShape slotButtons[3];
    sf::RectangleShape quitButton;

    // Botones para borrar cada ranura.
    sf::RectangleShape deleteButtons[3];
    sf::Text deleteTexts[3];

    // Textos del menu.
    sf::Text titleText;
    sf::Text newGameText;
    sf::Text slotTexts[3];
    sf::Text quitText;

    // Organiza posiciones y tamanos segun ventana.
    void layout(sf::RenderWindow& window);

    // Configuracion repetida de textos.
    void setupText(sf::Text& text, int size);

    // Centra un texto en una posicion.
    void centerText(sf::Text& text, sf::Vector2f center);

    // Fondo visual del menu.
    void drawBackground(sf::RenderWindow& window);

    // Cambia colores si el mouse esta encima.
    void updateHover(sf::RenderWindow& window);

public:
    MainMenu();

    // Procesa clicks y devuelve una accion para Game.
    MenuAction handleEvent(sf::Event& event, sf::RenderWindow& window);

    // Dibuja menu y resumen de partidas guardadas.
    void draw(sf::RenderWindow& window, const SaveSystem& saves);
};

#endif
