#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

// Proyectil normal usado por Shooter y Boss.
// Es struct porque sus datos son simples y se acceden directo en varias colisiones.
struct Projectile {
    // Hitbox circular del proyectil.
    sf::CircleShape shape;

    // Sprite animado del proyectil.
    sf::Sprite sprite;

    // Textura compartida por todos los proyectiles.
    static sf::Texture texture;
    static bool textureLoaded;

    // Velocidad del proyectil.
    sf::Vector2f velocity;

    // Animacion por frames.
    int currentFrame;
    float animationTimer;
    float frameTime;

    Projectile(sf::Vector2f position, sf::Vector2f direction);

    // Mueve y anima.
    void update(float deltaTime);

    // Dibuja sprite o fallback.
    void draw(sf::RenderWindow& window);

    // Se usa para borrar proyectiles fuera de pantalla.
    bool isOutside(sf::RenderWindow& window);

    // Detecta impacto con jugador.
    bool hitsPlayer(Player& player);
};

#endif
