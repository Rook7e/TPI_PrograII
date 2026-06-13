#include "EnemyBoss.h"
#include "VectorMath.h"
#include <algorithm>
#include <iostream>
#include <cmath>

sf::Texture EnemyBoss::texture;
bool EnemyBoss::textureLoaded = false;

EnemyBoss::EnemyBoss(sf::Vector2f position)
    : EnemyBase(position, 36.f, 95.f, 25)
{
    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Entidades/boss.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Entidades/boss.png" << std::endl;
        }
    }

    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();
        float scale = 144.f / textureSize.x;

        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);

        hitbox.setFillColor(sf::Color::Transparent);
    } else {
        hitbox.setFillColor(sf::Color(120, 40, 160));
    }

    damageTimer = 0.f;
    damageCooldown = 0.7f;

    dashTimer = 0.f;
    dashCooldown = 2.4f;
    dashDuration = 0.50f;
    dashSpeed = 500.f;
    dashing = false;
    dashDirection = sf::Vector2f(0.f, 0.f);

    shootTimer = 0.f;
    shootCooldown = 1.4f;
    burstShotsLeft = 0;
    burstShotTimer = 0.f;
    burstShotDelay = 0.04f;
    lastShotDirection = sf::Vector2f(0.f, 0.f);

    throwTimer = 0.f;
    throwCooldown = 4.f;

    state = BossMoving;
    stateTimer = 0.f;

    chargeDuration = 0.5f;
    recoverDuration = 0.7f;

    fanShotCount = 20;
    fanShotIndex = 0;
    fanAngle = 100.f;
}

void EnemyBoss::syncSpritePosition() {
    sprite.setPosition(hitbox.getPosition());
}

void EnemyBoss::updateFacing(sf::Vector2f direction) {
    if (!textureLoaded) {
        return;
    }

    if (direction.x > 0.f) {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    } else if (direction.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }
}

sf::Vector2f EnemyBoss::rotateVector(sf::Vector2f vector, float degrees) {
    float radians = degrees * 3.14159265f / 180.f;

    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);

    return sf::Vector2f(
        vector.x * cosAngle - vector.y * sinAngle,
        vector.x * sinAngle + vector.y * cosAngle
    );
}

void EnemyBoss::update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    if (isDead()) {
        return;
    }

    damageTimer += deltaTime;
    dashTimer += deltaTime;
    shootTimer += deltaTime;
    throwTimer += deltaTime;
    stateTimer += deltaTime;

    sf::Vector2f directionToPlayer = player.getCenter() - hitbox.getPosition();
    sf::Vector2f direction = normalize(directionToPlayer);
    float distance = vectorLength(directionToPlayer);

    if (state == BossMoving) {
        if (distance > 180.f) {
            hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
        }

        if (dashTimer >= dashCooldown &&
            tileMap.hasLineOfSight(hitbox.getPosition(), player.getCenter())) {
            sf::Vector2f sideDirection(-direction.y, direction.x);

            if (std::rand() % 2 == 0) {
                sideDirection.x *= -1.f;
                sideDirection.y *= -1.f;
            }

            dashDirection = sideDirection;
            dashTimer = 0.f;
            stateTimer = 0.f;
            state = BossSideDashing;

            if (textureLoaded) {
                sprite.setColor(sf::Color(255, 160, 120));
            }
        }
    } else if (state == BossSideDashing) {
        hitbox.move(
            dashDirection.x * dashSpeed * deltaTime,
            dashDirection.y * dashSpeed * deltaTime
        );

        if (stateTimer >= dashDuration) {
            state = BossChargingShot;
            stateTimer = 0.f;
            lastShotDirection = player.getCenter() - hitbox.getPosition();

            if (textureLoaded) {
                sprite.setColor(sf::Color(140, 220, 255));
            }
        }
    } else if (state == BossChargingShot) {
        lastShotDirection = player.getCenter() - hitbox.getPosition();

        if (stateTimer >= chargeDuration) {
            state = BossBurstShooting;
            stateTimer = 0.f;
            fanShotIndex = 0;
            burstShotTimer = burstShotDelay;

            if (textureLoaded) {
                sprite.setColor(sf::Color(255, 255, 255));
            }
        }
    } else if (state == BossBurstShooting) {
        burstShotTimer += deltaTime;

        if (fanShotIndex < fanShotCount && burstShotTimer >= burstShotDelay) {
            float startAngle = -fanAngle / 2.f;
            float angleStep = fanAngle / (fanShotCount - 1);
            float angle = startAngle + angleStep * fanShotIndex;

            sf::Vector2f shotDirection = rotateVector(lastShotDirection, angle);

            projectiles.push_back(
                Projectile(hitbox.getPosition(), shotDirection)
            );

            fanShotIndex++;
            burstShotTimer = 0.f;
        }

        if (fanShotIndex >= fanShotCount) {
            state = BossRecovering;
            stateTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color(180, 180, 180));
            }
        }
    } else if (state == BossRecovering) {
        if (stateTimer >= recoverDuration) {
            state = BossMoving;
            stateTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color::White);
            }
        }
    }

    if (throwTimer >= throwCooldown && state == BossMoving) {
        sf::Vector2i furnitureTile;

        if (tileMap.findNearestFurniture(hitbox.getPosition(), furnitureTile)) {
            int furnitureId = tileMap.getFurnitureId(furnitureTile.x, furnitureTile.y);

            furnitureProjectiles.push_back(
                FurnitureProjectile(
                    tileMap.getBuildingTileset(),
                    hitbox.getPosition(),
                    player.getCenter() - hitbox.getPosition(),
                    furnitureId
                )
            );

            tileMap.removeFurniture(furnitureTile.x, furnitureTile.y);
            throwTimer = 0.f;
        }
    }

    syncSpritePosition();
    updateFacing(directionToPlayer);

    if (hitbox.getGlobalBounds().intersects(player.getBounds()) &&
        damageTimer >= damageCooldown) {
        player.takeDamage(1);
        damageTimer = 0.f;
    }

    updateProjectiles(deltaTime, player, aspiradora, window, tileMap);
    updateFurnitureProjectiles(deltaTime, player, aspiradora, window, tileMap);
}

void EnemyBoss::updateProjectiles(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].update(deltaTime);
    }

    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [&window, &player, &aspiradora](Projectile& projectile) {
                if (projectile.shape.getGlobalBounds().intersects(aspiradora.getBounds())) {
                    return true;
                }

                if (projectile.hitsPlayer(player)) {
                    player.takeDamage(1);
                    return true;
                }

                return projectile.isOutside(window);
            }
        ),
        projectiles.end()
    );
}

void EnemyBoss::updateFurnitureProjectiles(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    for (int i = 0; i < furnitureProjectiles.size(); i++) {
        furnitureProjectiles[i].update(deltaTime);
    }

    furnitureProjectiles.erase(
        std::remove_if(
            furnitureProjectiles.begin(),
            furnitureProjectiles.end(),
            [&window, &player, &aspiradora, &tileMap](FurnitureProjectile& projectile) {
                if (tileMap.checkCollision(projectile.sprite.getGlobalBounds())) {
                    return true;
                }

                if (projectile.sprite.getGlobalBounds().intersects(aspiradora.getBounds())) {
                    return true;
                }

                if (projectile.hitsPlayer(player)) {
                    player.takeDamage(1);
                    return true;
                }

                return projectile.isOutside(window);
            }
        ),
        furnitureProjectiles.end()
    );
}

void EnemyBoss::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(hitbox);
    }

    drawHealthBar(window, 100.f);

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].draw(window);
    }

    for (int i = 0; i < furnitureProjectiles.size(); i++) {
        furnitureProjectiles[i].draw(window);
    }
}
