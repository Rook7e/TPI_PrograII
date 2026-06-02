#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <iostream>

float vectorLength(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f normalize(sf::Vector2f vector) {
    float length = vectorLength(vector);

    if (length == 0.f) {
        return sf::Vector2f(0.f, 0.f);
    }

    return sf::Vector2f(vector.x / length, vector.y / length);
}

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction) {
    shape.setRadius(6.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(6.f, 6.f);
    shape.setPosition(position);

    direction = normalize(direction);

    velocity.x = direction.x * 300.f;
    velocity.y = direction.y * 300.f;
}

void Projectile::update(float deltaTime) {
    shape.move(velocity.x * deltaTime, velocity.y * deltaTime);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Projectile::isOutside(sf::RenderWindow& window) {
    sf::Vector2f position = shape.getPosition();
    sf::Vector2u size = window.getSize();

    return position.x < 0.f ||
           position.y < 0.f ||
           position.x > size.x ||
           position.y > size.y;
}

bool Projectile::hitsPlayer(Player& player) {
    return shape.getGlobalBounds().intersects(player.getBounds());
}

EnemyChaser::EnemyChaser(sf::Vector2f position) {
    shape.setRadius(22.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(22.f, 22.f);
    shape.setPosition(position);

    speed = 120.f;

     maxVida = 5;
     vida = maxVida;

    damageTimer = 0.f;
    damageCooldown = 0.8f;
}

void EnemyChaser::update(float deltaTime, Player& player) {
    if (isDead()) {
        return;
    }

    damageTimer += deltaTime;

    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f direction = playerCenter - shape.getPosition();
    direction = normalize(direction);

    shape.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);

    if (shape.getGlobalBounds().intersects(player.getBounds()) &&
        damageTimer >= damageCooldown) {
        player.takeDamage(1);
        damageTimer = 0.f;
    }
}

void EnemyChaser::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    window.draw(shape);

    sf::RectangleShape back(sf::Vector2f(44.f, 6.f));
    back.setFillColor(sf::Color(70, 20, 20));
    back.setPosition(shape.getPosition().x - 22.f, shape.getPosition().y - 35.f);

    float percent = (float)vida / maxVida;

    sf::RectangleShape bar(sf::Vector2f(44.f * percent, 6.f));
    bar.setFillColor(sf::Color::Red);
    bar.setPosition(back.getPosition());

    window.draw(back);
    window.draw(bar);
}

void EnemyChaser::takeDamage(int damage, sf::Vector2f hitPosition) {
    if (isDead()) {
        return;
    }

    vida -= damage;

    sf::Vector2f knockDirection = shape.getPosition() - hitPosition;
    knockDirection = normalize(knockDirection);

    float knockbackForce = 45.f;
    shape.move(knockDirection.x * knockbackForce, knockDirection.y * knockbackForce);

    if (vida < 0) {
        vida = 0;
    }

    std::cout << "Vida chaser: " << vida << std::endl;
}

bool EnemyChaser::isDead() {
    return vida <= 0;
}

sf::FloatRect EnemyChaser::getBounds() {
    return shape.getGlobalBounds();
}

EnemyShooter::EnemyShooter(sf::Vector2f position) {
    shape.setRadius(24.f);
    shape.setFillColor(sf::Color::Magenta);
    shape.setOrigin(24.f, 24.f);
    shape.setPosition(position);

    speed = 90.f;
    desiredDistance = 220.f;

    shootTimer = 0.f;
    shootCooldown = 1.5f;

     maxVida = 5;
     vida = maxVida;
}

void EnemyShooter::update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window,TileMap& tileMap) {
    if (isDead()) {
        return;
    }




    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f directionToPlayer = playerCenter - shape.getPosition();
    float distance = vectorLength(directionToPlayer);

    sf::Vector2f direction = normalize(directionToPlayer);

    if (distance > desiredDistance + 30.f) {
        shape.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    }

    if (distance < desiredDistance - 30.f) {
        shape.move(-direction.x * speed * deltaTime, -direction.y * speed * deltaTime);
    }

    shootTimer += deltaTime;

    if (shootTimer >= shootCooldown) {
        projectiles.push_back(Projectile(shape.getPosition(), directionToPlayer));
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

                if (tileMap.checkCollision(projectile.shape.getGlobalBounds())){
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

    window.draw(shape);

    sf::RectangleShape back(sf::Vector2f(48.f, 6.f));
    back.setFillColor(sf::Color(70, 20, 20));
    back.setPosition(shape.getPosition().x - 24.f, shape.getPosition().y - 38.f);

    float percent = (float)vida / maxVida;

    sf::RectangleShape bar(sf::Vector2f(48.f * percent, 6.f));
    bar.setFillColor(sf::Color::Red);
    bar.setPosition(back.getPosition());

    window.draw(back);
    window.draw(bar);

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].draw(window);
    }
}

void EnemyShooter::takeDamage(int damage, sf::Vector2f hitPosition) {
    if (isDead()) {
        return;
    }

    vida -= damage;

    sf::Vector2f knockDirection = shape.getPosition() - hitPosition;
    knockDirection = normalize(knockDirection);

    float knockbackForce = 45.f;
    shape.move(knockDirection.x * knockbackForce, knockDirection.y * knockbackForce);

    if (vida < 0) {
        vida = 0;
    }

    std::cout << "Vida shooter: " << vida << std::endl;
}

bool EnemyShooter::isDead() {
    return vida <= 0;
}

sf::FloatRect EnemyShooter::getBounds() {
    return shape.getGlobalBounds();
}
