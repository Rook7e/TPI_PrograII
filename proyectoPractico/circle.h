#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "MangueraAspiradora.h"

// Esta clase representa la punta de la aspiradora.
// Se llama circle por una version anterior, pero funciona como herramienta del jugador.
class circle {
private:
    // Hitbox circular usada para limpiar mugre, hacer dano y bloquear proyectiles.
    sf::CircleShape shape;

    // Punto donde se engancha la manguera al jugador.
    sf::Vector2f attachPoint;

    // Distancia maxima entre jugador y punta de aspiradora.
    float maxDistance;

    // Sprite visual de la punta.
    sf::Sprite sprite;
    sf::Texture texture;
    bool textureLoaded;

    // Manguera visual entre jugador y punta.
    MangueraAspiradora manguera;

public:
    circle();

    // Actualiza posicion segun mouse y limita distancia al jugador.
    void update(sf::RenderWindow& window, sf::Vector2f playerCenter);

    // Dibuja manguera, conectores y punta.
    void draw(sf::RenderWindow& window);

    // Permite mejorar el rango desde el sistema de upgrades.
    void setMaxDistance(float distance);

    // Bounds de la punta para colisiones.
    sf::FloatRect getBounds();

    // Posicion actual de la punta.
    sf::Vector2f getPosition();
};

#endif
