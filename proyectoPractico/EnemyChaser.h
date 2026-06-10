#ifndef ENEMYCHASER_H_INCLUDED
#define ENEMYCHASER_H_INCLUDED

#pragma once
#include "EnemyBase.h"
#include "Player.h"

enum ChaserState {
    Chasing,
    ChargingDash,
    Dashing,
    Recovering
};

class EnemyChaser : public EnemyBase {
private:
    sf::Sprite sprite;

    static sf::Texture texture;
    static bool textureLoaded;

    float damageTimer;
    float damageCooldown;

    ChaserState state;
    sf::Vector2f dashDirection;

    float stateTimer;
    float dashCooldownTimer;

    float dashSpeed;
    float chargeDuration;
    float dashDuration;
    float recoverDuration;
    float dashCooldown;

    void updateFacing(sf::Vector2f direction);

protected:
    void syncSpritePosition();

public:
    EnemyChaser(sf::Vector2f position);

    void update(float deltaTime, Player& player);
    void draw(sf::RenderWindow& window);
};

#endif
