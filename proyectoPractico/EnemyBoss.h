#ifndef ENEMYBOSS_H_INCLUDED
#define ENEMYBOSS_H_INCLUDED

#pragma once
#include <vector>
#include "EnemyBase.h"
#include "Projectile.h"
#include "FurnitureProjectile.h"
#include "Player.h"
#include "circle.h"
#include "TileMap.h"

enum BossState {
    BossMoving,
    BossSideDashing,
    BossWallDashCharging,
    BossWallDashing,
    BossChargingShot,
    BossBurstShooting,
    BossRecovering
};

class EnemyBoss : public EnemyBase {
private:
    sf::Sprite sprite;

    static sf::Texture texture;
    static bool textureLoaded;

    std::vector<Projectile> projectiles;
    std::vector<FurnitureProjectile> furnitureProjectiles;

    float damageTimer;
    float damageCooldown;

    float dashTimer;
    float dashCooldown;
    float dashDuration;
    float dashSpeed;
    bool dashing;
    sf::Vector2f dashDirection;

    float shootTimer;
    float shootCooldown;
    int burstShotsLeft;
    float burstShotTimer;
    float burstShotDelay;
    sf::Vector2f lastShotDirection;

    BossState state;

    float stateTimer;
    float chargeDuration;
    float recoverDuration;

    int fanShotCount;
    int fanShotIndex;
    float fanAngle;

    float throwTimer;
    float throwCooldown;

    bool phaseTwo;

    int wallDashCount;
    int maxWallDashes;

    float wallDashChargeDuration;
    float wallDashMaxDuration;
    float wallDashSpeed;

    sf::Vector2f wallDashDirection;

    bool tryMove(sf::Vector2f movement, sf::RenderWindow& window, TileMap& tileMap);
    void startWallDash(sf::Vector2f targetPosition);

    void updateFacing(sf::Vector2f direction);
    void updateProjectiles(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap);
    void updateFurnitureProjectiles(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap);
    sf::Vector2f rotateVector(sf::Vector2f vector, float degrees);



protected:
    void syncSpritePosition();

public:
    EnemyBoss(sf::Vector2f position);

    float getLifePercent();

    void update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap);
    void draw(sf::RenderWindow& window);
};

#endif
