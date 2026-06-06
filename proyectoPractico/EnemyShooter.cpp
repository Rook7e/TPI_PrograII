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
        if (shooterTexture.loadFromFile("assets/Shooter.png")) {
            shooterTextureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Shooter.png" << std::endl;
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
    shootCooldown = 1.5f;
}

void EnemyShooter::syncSpritePosition() {
    sprite.setPosition(hitbox.getPosition());
}

void EnemyShooter::update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    if (isDead()) {
        return;
    }

    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f directionToPlayer = playerCenter - hitbox.getPosition();
    float distance = vectorLength(directionToPlayer);

    sf::Vector2f direction = normalize(directionToPlayer);

    if (distance > desiredDistance + 30.f) {
        hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    }

    if (distance < desiredDistance - 30.f) {
        hitbox.move(-direction.x * speed * deltaTime, -direction.y * speed * deltaTime);
    }

    syncSpritePosition();

    if (shooterTextureLoaded) {
        if (directionToPlayer.x > 0.f) {
            sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
        } else if (directionToPlayer.x < 0.f) {
            sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
        }
    }

    shootTimer += deltaTime;

    if (shootTimer >= shootCooldown) {
        projectiles.push_back(Projectile(hitbox.getPosition(), directionToPlayer));
        shootTimer = 0.f;
    }

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
