#ifndef FURNITUREPROJECTILE_H_INCLUDED
#define FURNITUREPROJECTILE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

// Proyectil creado a partir de un mueble del mapa.
// Lo usa el Thrower y tambien el Boss.
struct FurnitureProjectile {
    // Sprite del mueble lanzado.
    sf::Sprite sprite;

    // Velocidad del proyectil.
    sf::Vector2f velocity;

    FurnitureProjectile(
        const sf::Texture& tileset,
        sf::Vector2f position,
        sf::Vector2f direction,
        int furnitureId
    );

    // Mueve el mueble.
    void update(float deltaTime);

    // Dibuja el mueble.
    void draw(sf::RenderWindow& window);

    // Permite borrarlo si sale de pantalla.
    bool isOutside(sf::RenderWindow& window);

    // Detecta impacto contra jugador.
    bool hitsPlayer(Player& player);
};

#endif
