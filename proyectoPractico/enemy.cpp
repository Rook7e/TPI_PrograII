#include "Enemy.h"
#include <cmath>
#include <algorithm>
#include <iostream>

sf::Texture EnemyChaser::texture;
bool EnemyChaser::textureLoaded = false;

sf::Texture Projectile::texture;
bool Projectile::textureLoaded = false;

sf::Texture EnemyShooter::shooterTexture;
bool EnemyShooter::shooterTextureLoaded = false;

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
    shape.setRadius(8.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(8.f, 8.f);
    shape.setPosition(position);

    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Ectoplasma.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar Ectoplasma.png" << std::endl;
            shape.setFillColor(sf::Color::Yellow);
        }
    }

    currentFrame = 0;
    animationTimer = 0.f;
    frameTime = 0.08f;

    if (textureLoaded) {
        sprite.setTexture(texture);

        int frameWidth = texture.getSize().x / 4;
        int frameHeight = texture.getSize().y;

        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);

        float scale = 20.f / frameWidth;
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
    }

    direction = normalize(direction);

    velocity.x = direction.x * 300.f;
    velocity.y = direction.y * 300.f;
}

void Projectile::update(float deltaTime) {
    shape.move(velocity.x * deltaTime, velocity.y * deltaTime);

    if (textureLoaded) {
        sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);

        animationTimer += deltaTime;

        if (animationTimer >= frameTime) {
            currentFrame++;

            if (currentFrame >= 4) {
                currentFrame = 0;
            }

            int frameWidth = texture.getSize().x / 4;
            int frameHeight = texture.getSize().y;

            sprite.setTextureRect(sf::IntRect(
                currentFrame * frameWidth,
                0,
                frameWidth,
                frameHeight
            ));

            animationTimer = 0.f;
        }
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
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
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(22.f, 22.f);
    shape.setPosition(position);

    if (!textureLoaded) {
        if (!texture.loadFromFile("assets/boo.png")) {
            std::cout << "Error al cargar boo.png" << std::endl;
        }

        textureLoaded = true;
    }

    sprite.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize();

    if (textureSize.x > 0) {
        float scale = 44.f / textureSize.x;
        sprite.setScale(scale, scale);
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
    }

    sprite.setPosition(position);

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
    sf::Vector2f rawDirection = playerCenter - shape.getPosition();
    sf::Vector2f direction = normalize(rawDirection);

    shape.move(direction.x * speed * deltaTime, direction.y * speed * deltaTime);
    sprite.setPosition(shape.getPosition());

    if (rawDirection.x > 0.f) {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    } else if (rawDirection.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }

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

    window.draw(sprite);

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
    sprite.setPosition(shape.getPosition());
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
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

        float scale = 48.f / textureSize.x;
        sprite.setScale(scale, scale);
        sprite.setPosition(position);

        shape.setFillColor(sf::Color::Transparent);
    }

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

    sprite.setPosition(shape.getPosition());

    if (directionToPlayer.x > 0.f) {
    sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    } else if (directionToPlayer.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
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

    if (shooterTextureLoaded) {
    window.draw(sprite);
} else {
    window.draw(shape);
}

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

    sprite.setPosition(shape.getPosition());

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
