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

    fanShotCount = 25;
    fanShotIndex = 0;
    fanAngle = 100.f;

    phaseTwo = false;

    wallDashCount = 0;
    maxWallDashes = 3;

    wallDashChargeDuration = 0.35f;
    wallDashMaxDuration = 1.2f;
    wallDashSpeed = 620.f;

    wallDashDirection = sf::Vector2f(0.f, 0.f);
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

    if (!phaseTwo && getLifePercent() <= 0.5f) {
    phaseTwo = true;

    dashCooldown = 1.4f;
    shootCooldown = 0.9f;
    fanShotCount = 28;
    fanAngle = 135.f;

    wallDashCount = 0;
    startWallDash(player.getCenter());

    return;
}

    if (state == BossMoving) {
    if (distance > 180.f) {
        sf::Vector2f movement(
            direction.x * speed * deltaTime,
            direction.y * speed * deltaTime
        );

        tryMove(movement, window, tileMap);
    }

        if (dashTimer >= dashCooldown &&
            tileMap.hasLineOfSight(hitbox.getPosition(), player.getCenter())) {

            dashTimer = 0.f;
            stateTimer = 0.f;

            if (phaseTwo && std::rand() % 2 == 0) {
                wallDashCount = 0;
                startWallDash(player.getCenter());
            } else {
                sf::Vector2f sideDirection(-direction.y, direction.x);

                if (std::rand() % 2 == 0) {
                    sideDirection.x *= -1.f;
                    sideDirection.y *= -1.f;
                }

                dashDirection = sideDirection;
                state = BossSideDashing;

                if (textureLoaded) {
                    sprite.setColor(sf::Color(255, 160, 120));
                }
            }
        }
    }
                else if (state == BossSideDashing) {
                sf::Vector2f movement(
                    dashDirection.x * dashSpeed * deltaTime,
                    dashDirection.y * dashSpeed * deltaTime
                );

                bool moved = tryMove(movement, window, tileMap);

                if (!moved || stateTimer >= dashDuration) {
                    state = BossChargingShot;
                    stateTimer = 0.f;
                    lastShotDirection = player.getCenter() - hitbox.getPosition();

                    if (textureLoaded) {
                        sprite.setColor(sf::Color(140, 220, 255));
                    }
                }
            }

                else if (state == BossWallDashCharging) {
            wallDashDirection = normalize(player.getCenter() - hitbox.getPosition());
            updateFacing(wallDashDirection);

            if (stateTimer >= wallDashChargeDuration) {
                state = BossWallDashing;
                stateTimer = 0.f;

                if (textureLoaded) {
                    sprite.setColor(sf::Color(255, 255, 255));
                }
            }
        }

    else if (state == BossWallDashing) {
            sf::Vector2f movement(
                wallDashDirection.x * wallDashSpeed * deltaTime,
                wallDashDirection.y * wallDashSpeed * deltaTime
        );

        bool moved = tryMove(movement, window, tileMap);

        if (!moved || stateTimer >= wallDashMaxDuration) {
            wallDashCount++;

            if (wallDashCount < maxWallDashes) {
                    startWallDash(player.getCenter());
            } else {
                state = BossRecovering;
                stateTimer = 0.f;

                if (textureLoaded) {
                    sprite.setColor(sf::Color(180, 180, 180));
                }
            }
        }
    }

     else if (state == BossChargingShot) {
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
    }
     else if (state == BossBurstShooting) {
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
    float EnemyBoss::getLifePercent() {
    if (maxVida <= 0) {
        return 0.f;
    }

    return (float)vida / maxVida;
}

bool EnemyBoss::tryMove(sf::Vector2f movement, sf::RenderWindow& window, TileMap& tileMap) {
    sf::FloatRect nextBounds = hitbox.getGlobalBounds();

    nextBounds.left += movement.x;
    nextBounds.top += movement.y;

    if (nextBounds.left < 0.f) {
        hitbox.setPosition(
            hitbox.getPosition().x - nextBounds.left,
            hitbox.getPosition().y
        );
        syncSpritePosition();
        return false;
    }

    if (nextBounds.top < 0.f) {
        hitbox.setPosition(
            hitbox.getPosition().x,
            hitbox.getPosition().y - nextBounds.top
        );
        syncSpritePosition();
        return false;
    }

    if (nextBounds.left + nextBounds.width > window.getSize().x) {
        float overflow = (nextBounds.left + nextBounds.width) - window.getSize().x;

        hitbox.setPosition(
            hitbox.getPosition().x - overflow,
            hitbox.getPosition().y
        );
        syncSpritePosition();
        return false;
    }

    if (nextBounds.top + nextBounds.height > window.getSize().y) {
        float overflow = (nextBounds.top + nextBounds.height) - window.getSize().y;

        hitbox.setPosition(
            hitbox.getPosition().x,
            hitbox.getPosition().y - overflow
        );
        syncSpritePosition();
        return false;
    }

    hitbox.move(movement);
    syncSpritePosition();

    return true;
}

void EnemyBoss::startWallDash(sf::Vector2f targetPosition) {
    wallDashDirection = normalize(targetPosition - hitbox.getPosition());

    if (vectorLength(wallDashDirection) == 0.f) {
        wallDashDirection = sf::Vector2f(1.f, 0.f);
    }

    state = BossWallDashCharging;
    stateTimer = 0.f;

    updateFacing(wallDashDirection);

    if (textureLoaded) {
        sprite.setColor(sf::Color(255, 80, 80));
    }
}
