#ifndef ENEMYCHASER_H_INCLUDED
#define ENEMYCHASER_H_INCLUDED

#pragma once
#include "EnemyBase.h"
#include "Player.h"

class EnemyChaser : public EnemyBase {
private:
    sf::Sprite sprite;

    static sf::Texture texture;
    static bool textureLoaded;

    float damageTimer;
    float damageCooldown;

protected:
    void syncSpritePosition();

public:
    EnemyChaser(sf::Vector2f position);

    void update(float deltaTime, Player& player);
    void draw(sf::RenderWindow& window);
};

#endif
