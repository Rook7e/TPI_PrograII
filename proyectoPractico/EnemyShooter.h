#ifndef ENEMYSHOOTER_H_INCLUDED
#define ENEMYSHOOTER_H_INCLUDED

#pragma once
#include <vector>
#include "EnemyBase.h"
#include "Projectile.h"
#include "Player.h"
#include "circle.h"
#include "TileMap.h"

class EnemyShooter : public EnemyBase {
private:
    sf::Sprite sprite;

    static sf::Texture shooterTexture;
    static bool shooterTextureLoaded;

    std::vector<Projectile> projectiles;

    float desiredDistance;
    float shootTimer;
    float shootCooldown;

protected:
    void syncSpritePosition();

public:
    EnemyShooter(sf::Vector2f position);

    void update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap);
    void draw(sf::RenderWindow& window);
};

#endif
