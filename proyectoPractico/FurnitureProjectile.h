#ifndef FURNITUREPROJECTILE_H_INCLUDED
#define FURNITUREPROJECTILE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

struct FurnitureProjectile {
    sf::Sprite sprite;
    sf::Vector2f velocity;

    FurnitureProjectile(
        const sf::Texture& tileset,
        sf::Vector2f position,
        sf::Vector2f direction,
        int furnitureId
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isOutside(sf::RenderWindow& window);
    bool hitsPlayer(Player& player);
};

#endif
