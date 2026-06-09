#ifndef ENEMYTHROWER_H_INCLUDED
#define ENEMYTHROWER_H_INCLUDED

#pragma once
#include <vector>
#include "EnemyBase.h"
#include "FurnitureProjectile.h"
#include "Player.h"
#include "circle.h"
#include "TileMap.h"

class EnemyThrower : public EnemyBase {
private:
    sf::Sprite sprite;

    static sf::Texture texture;
    static bool textureLoaded;

    std::vector<FurnitureProjectile> furnitureProjectiles;

    float throwTimer;
    float throwCooldown;

    float damageTimer;
    float damageCooldown;

protected:
    void syncSpritePosition();

public:
    EnemyThrower(sf::Vector2f position);

    void update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap);
    void draw(sf::RenderWindow& window);
    bool holdingFurniture;
    int carriedFurnitureId;
    bool targetingFurniture;
    sf::Vector2i targetFurnitureTile;
};

#endif
