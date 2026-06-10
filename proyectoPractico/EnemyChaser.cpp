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
        if (texture.loadFromFile("assets/Entidades/chasser.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/chasser.png" << std::endl;
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

    state = Chasing;
    dashDirection = sf::Vector2f(0.f, 0.f);

    stateTimer = 0.f;
    dashCooldownTimer = 0.f;

    dashSpeed = 340.f;
    chargeDuration = 0.35f;
    dashDuration = 0.28f;
    recoverDuration = 0.45f;
    dashCooldown = 2.2f;
}

void EnemyChaser::syncSpritePosition() {
    sprite.setPosition(hitbox.getPosition());
}

void EnemyChaser::updateFacing(sf::Vector2f direction) {
    if (!textureLoaded) {
        return;
    }

    if (direction.x > 0.f) {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    } else if (direction.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }
}

void EnemyChaser::update(float deltaTime, Player& player) {
    if (isDead()) {
        return;
    }

    damageTimer += deltaTime;
    dashCooldownTimer += deltaTime;
    stateTimer += deltaTime;

    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f rawDirection = playerCenter - hitbox.getPosition();
    sf::Vector2f direction = normalize(rawDirection);
    float distance = vectorLength(rawDirection);

    bool horizontallyAligned = std::abs(rawDirection.y) < 45.f;
    bool verticallyAligned = std::abs(rawDirection.x) < 45.f;
    bool canDash = dashCooldownTimer >= dashCooldown &&
                   distance < 260.f &&
                   (horizontallyAligned || verticallyAligned);

    if (state == Chasing) {
        hitbox.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);

        if (canDash) {
            state = ChargingDash;
            stateTimer = 0.f;
            dashDirection = direction;
        }
    } else if (state == ChargingDash) {
        if (textureLoaded) {
            sprite.setColor(sf::Color(255, 120, 120));
        }

        if (stateTimer >= chargeDuration) {
            state = Dashing;
            stateTimer = 0.f;
        }
    } else if (state == Dashing) {
        hitbox.move(
            dashDirection.x * dashSpeed * deltaTime,
            dashDirection.y * dashSpeed * deltaTime
        );

        if (stateTimer >= dashDuration) {
            state = Recovering;
            stateTimer = 0.f;
        }
    } else if (state == Recovering) {
        if (textureLoaded) {
            sprite.setColor(sf::Color(180, 180, 180));
        }

        if (stateTimer >= recoverDuration) {
            state = Chasing;
            stateTimer = 0.f;
            dashCooldownTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color::White);
            }
        }
    }

    syncSpritePosition();
    updateFacing(rawDirection);

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
