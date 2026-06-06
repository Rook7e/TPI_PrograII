#include "EnemyChaser.h"
#include "VectorMath.h"
#include <iostream>
#include <cmath>

sf::Texture EnemyChaser::texture;
bool EnemyChaser::textureLoaded = false;

EnemyChaser::EnemyChaser(sf::Vector2f position)
    : EnemyBase(position, 22.f, 120.f, 5)
{
    if (!textureLoaded) {
        if (texture.loadFromFile("assets/boo.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/boo.png" << std::endl;
        }
    }

    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();
        float scale = 44.f / textureSize.x;

        sprite.setScale(scale, scale);
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        sprite.setPosition(position);
        hitbox.setFillColor(sf::Color::Transparent);
    } else {
        hitbox.setFillColor(sf::Color::Red);
    }

    damageTimer = 0.f;
    damageCooldown = 0.8f;
}

void EnemyChaser::syncSpritePosition() {
    sprite.setPosition(hitbox.getPosition());
}

void EnemyChaser::update(float deltaTime, Player& player) {
    if (isDead()) {
        return;
    }

    damageTimer += deltaTime;

    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f rawDirection = playerCenter - hitbox.getPosition();
    sf::Vector2f direction = normalize(rawDirection);

    hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    syncSpritePosition();

    if (textureLoaded) {
        if (rawDirection.x > 0.f) {
            sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
        } else if (rawDirection.x < 0.f) {
            sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
        }
    }

    if (hitbox.getGlobalBounds().intersects(player.getBounds()) &&
        damageTimer >= damageCooldown) {
        player.takeDamage(1);
        damageTimer = 0.f;
    }
}

void EnemyChaser::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(hitbox);
    }

    drawHealthBar(window, 44.f);
}
