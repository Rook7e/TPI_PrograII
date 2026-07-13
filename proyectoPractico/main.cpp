#include "Game.h"

int main() {
    // Creamos el objeto principal del juego.
    // La clase Game contiene la ventana, el loop principal,
    // los menus, el jugador, enemigos, mapas y demas sistemas.
    Game game;

    // Inicia el ciclo principal del juego.
    // Mientras la ventana este abierta, Game se encarga de:
    // procesar eventos, actualizar logica y dibujar.
    game.run();

    return 0;
}
