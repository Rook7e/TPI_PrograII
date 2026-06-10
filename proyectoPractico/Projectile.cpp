#include "Projectile.h"
#include "VectorMath.h"
#include <iostream>

sf::Texture Projectile::texture;
bool Projectile::textureLoaded = false;

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction) {
    shape.setRadius(8.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(8.f, 8.f);
    shape.setPosition(position);

    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Extras/Ectoplasma.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Ectoplasma.png" << std::endl;
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

            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
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
