#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

struct Projectile {
    sf::CircleShape shape;
    sf::Sprite sprite;

    static sf::Texture texture;
    static bool textureLoaded;

    sf::Vector2f velocity;

    int currentFrame;
    float animationTimer;
    float frameTime;

    Projectile(sf::Vector2f position, sf::Vector2f direction);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isOutside(sf::RenderWindow& window);
    bool hitsPlayer(Player& player);
};

#endif
