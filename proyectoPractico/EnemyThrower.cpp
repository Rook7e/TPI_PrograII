#include "EnemyThrower.h"
#include "VectorMath.h"
#include <algorithm>
#include <iostream>
#include <cmath>

sf::Texture EnemyThrower::texture;
bool EnemyThrower::textureLoaded = false;

EnemyThrower::EnemyThrower(sf::Vector2f position)
    : EnemyBase(position, 24.f, 80.f, 5)
{
    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Entidades/thrower.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Entidades/thrower.png" << std::endl;
        }
    }

    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();
        float scale = 48.f / textureSize.x;

        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);

        hitbox.setFillColor(sf::Color::Transparent);
    } else {
        hitbox.setFillColor(sf::Color::Blue);
    }

    throwTimer = 0.f;
    throwCooldown = 2.f;
}

void EnemyThrower::syncSpritePosition() {
    sprite.setPosition(hitbox.getPosition());
}

void EnemyThrower::update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window, TileMap& tileMap) {
    if (isDead()) {
        return;
    }

    throwTimer += deltaTime;

    sf::Vector2i furnitureTile;

    bool foundFurniture = tileMap.findNearestFurniture(hitbox.getPosition(), furnitureTile);

    if (foundFurniture) {
        sf::Vector2f furniturePos(
            furnitureTile.x * 32.f + 16.f,
            furnitureTile.y * 32.f + 16.f
        );

        sf::Vector2f direction = normalize(furniturePos - hitbox.getPosition());

        hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
        syncSpritePosition();

        float distance = vectorLength(furniturePos - hitbox.getPosition());

        if (distance < 25.f && throwTimer >= throwCooldown) {
            furnitureProjectiles.push_back(
                FurnitureProjectile(
                    tileMap.getTileset(),
                    hitbox.getPosition(),
                    player.getCenter() - hitbox.getPosition()
                )
            );

            tileMap.removeFurniture(furnitureTile.x, furnitureTile.y);
            throwTimer = 0.f;
        }
    } else {
        sf::Vector2f direction = normalize(player.getCenter() - hitbox.getPosition());

        hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
        syncSpritePosition();
    }

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

void EnemyThrower::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(hitbox);
    }

    for (int i = 0; i < furnitureProjectiles.size(); i++) {
        furnitureProjectiles[i].draw(window);
    }

    drawHealthBar(window, 48.f);
}
