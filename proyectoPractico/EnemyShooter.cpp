#include "EnemyShooter.h"
#include "VectorMath.h"
#include <algorithm>
#include <iostream>
#include <cmath>

sf::Texture EnemyShooter::shooterTexture;
bool EnemyShooter::shooterTextureLoaded = false;

EnemyShooter::EnemyShooter(sf::Vector2f position)
    : EnemyBase(position, 24.f, 90.f, 5)
{
    if (!shooterTextureLoaded) {
        if (shooterTexture.loadFromFile("assets/Entidades/Shooter.png")) {
            shooterTextureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Entidades/Shooter.png" << std::endl;
        }
    }

    if (shooterTextureLoaded) {
        sprite.setTexture(shooterTexture);

        sf::Vector2u textureSize = shooterTexture.getSize();
        float scale = 48.f / textureSize.x;

        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
        hitbox.setFillColor(sf::Color::Transparent);
    } else {
        hitbox.setFillColor(sf::Color::Magenta);
    }

    desiredDistance = 220.f;

    shootTimer = 0.f;
    shootCooldown = 1.6f;

    chargeDuration = 0.45f;
    recoverDuration = 0.6f;

    burstShotsLeft = 0;
    burstShotIndex = 0;
    burstShotTimer = 0.f;
    burstShotDelay = 0.14f;

    lastShotDirection = sf::Vector2f(0.f, 0.f);

    state = Repositioning;
}

void EnemyShooter::syncSpritePosition() {
    sprite.setPosition(hitbox.getPosition());
}

void EnemyShooter::updateFacing(sf::Vector2f direction) {
    if (!shooterTextureLoaded) {
        return;
    }

    if (direction.x > 0.f) {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    } else if (direction.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }
}

sf::Vector2f EnemyShooter::rotateVector(sf::Vector2f vector, float degrees) {
    float radians = degrees * 3.14159265f / 180.f;

    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);

    return sf::Vector2f(
        vector.x * cosAngle - vector.y * sinAngle,
        vector.x * sinAngle + vector.y * cosAngle
    );
}

void EnemyShooter::update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    if (isDead()) {
        return;
    }

    shootTimer += deltaTime;

    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f directionToPlayer = playerCenter - hitbox.getPosition();
    sf::Vector2f direction = normalize(directionToPlayer);
    float distance = vectorLength(directionToPlayer);

    bool hasVision = tileMap.hasLineOfSight(hitbox.getPosition(), playerCenter);

    if (state == Repositioning) {
        if (distance > desiredDistance + 30.f) {
            hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
        } else if (distance < desiredDistance - 30.f) {
            hitbox.move(-direction.x * speed * deltaTime, -direction.y * speed * deltaTime);
        }

        if (hasVision &&
            distance <= desiredDistance + 60.f &&
            shootTimer >= shootCooldown) {
            state = ChargingShot;
            shootTimer = 0.f;
            lastShotDirection = directionToPlayer;

            if (shooterTextureLoaded) {
                sprite.setColor(sf::Color(140, 220, 255));
            }
        }
    } else if (state == ChargingShot) {
                if (shootTimer >= chargeDuration) {
                    state = BurstShooting;
                    burstShotsLeft = 3;
                    burstShotIndex = 0;
                    burstShotTimer = burstShotDelay;
                    shootTimer = 0.f;
                    lastShotDirection = player.getCenter() - hitbox.getPosition();
                }
            } else if (state == BurstShooting) {
                burstShotTimer += deltaTime;

                if (burstShotsLeft > 0 && burstShotTimer >= burstShotDelay) {
            float angles[3] = { -12.f, 0.f, 12.f };

            sf::Vector2f shotDirection = rotateVector(
                lastShotDirection,
                angles[burstShotIndex]
            );

            projectiles.push_back(Projectile(hitbox.getPosition(), shotDirection));

            burstShotIndex++;
            burstShotsLeft--;
            burstShotTimer = 0.f;
        }

        if (burstShotsLeft <= 0) {
            state = ShotRecovering;
            shootTimer = 0.f;

            if (shooterTextureLoaded) {
                sprite.setColor(sf::Color(180, 180, 180));
            }
        }
    } else if (state == ShotRecovering) {
        if (shootTimer >= recoverDuration) {
            state = Repositioning;
            shootTimer = 0.f;

            if (shooterTextureLoaded) {
                sprite.setColor(sf::Color::White);
            }
        }
    }

    syncSpritePosition();
    updateFacing(directionToPlayer);

    updateProjectiles(deltaTime, player, aspiradora, window, tileMap);
}

void EnemyShooter::updateProjectiles(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].update(deltaTime);
    }

    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [&window, &player, &aspiradora, &tileMap](Projectile& projectile) {
                if (tileMap.checkCollision(projectile.shape.getGlobalBounds())) {
                    return true;
                }

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

void EnemyShooter::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    if (shooterTextureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(hitbox);
    }

    drawHealthBar(window, 48.f);

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].draw(window);
    }
}
