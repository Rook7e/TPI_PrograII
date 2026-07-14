#ifndef MEDKIT_H_INCLUDED
#define MEDKIT_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

// Botiquin que aparece en la sala y cura al jugador al tocarlo.
class Medkit {
private:
    // Caja roja del botiquin.
    sf::RectangleShape box;

    // Cruz blanca dibujada con dos rectangulos.
    sf::RectangleShape crossVertical;
    sf::RectangleShape crossHorizontal;

public:
    Medkit(sf::Vector2f position);

    // Dibuja el botiquin.
    void draw(sf::RenderWindow& window);

    // Bounds para detectar contacto con jugador.
    sf::FloatRect getBounds();
};

#endif
