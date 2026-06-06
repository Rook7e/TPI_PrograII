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
    std::vector<FurnitureProjectile> furnitureProjectiles;

    float throwTimer;
    float throwCooldown;

public:
    EnemyThrower(sf::Vector2f position);

    void update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap);
    void draw(sf::RenderWindow& window);
};

#endif
