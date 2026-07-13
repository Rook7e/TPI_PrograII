#include "Projectile.h"
#include "VectorMath.h"
#include <iostream>

// Textura compartida por todos los proyectiles.
sf::Texture Projectile::texture;
bool Projectile::textureLoaded = false;

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction) {
    // Hitbox circular del proyectil.
    shape.setRadius(8.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(8.f, 8.f);
    shape.setPosition(position);

    // Cargamos la textura una sola vez.
    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Extras/Ectoplasma.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Extras/Ectoplasma.png" << std::endl;
            shape.setFillColor(sf::Color::Yellow);
        }
    }

    // Datos de animacion.
    currentFrame = 0;
    animationTimer = 0.f;
    frameTime = 0.08f;

    if (textureLoaded) {
        sprite.setTexture(texture);

        // La imagen tiene 4 frames horizontales.
        int frameWidth = texture.getSize().x / 4;
        int frameHeight = texture.getSize().y;

        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);

        float scale = 20.f / frameWidth;
        sprite.setScale(scale, scale);
        sprite.setPosition(position);
    }

    // Normalizamos direccion y asignamos velocidad.
    direction = normalize(direction);

    velocity.x = direction.x * 300.f;
    velocity.y = direction.y * 300.f;
}

void Projectile::update(float deltaTime) {
    // Movemos hitbox.
    shape.move(velocity.x * deltaTime, velocity.y * deltaTime);

    if (textureLoaded) {
        // Movemos sprite.
        sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);

        // Animacion por frames.
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
