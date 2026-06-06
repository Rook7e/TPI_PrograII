#include "FurnitureProjectile.h"
#include "VectorMath.h"

FurnitureProjectile::FurnitureProjectile(
    const sf::Texture& tileset,
    sf::Vector2f position,
    sf::Vector2f direction)
{
    sprite.setTexture(tileset);
    sprite.setTextureRect(sf::IntRect(384, 0, 32, 32));
    sprite.setOrigin(16.f, 16.f);
    sprite.setPosition(position);

    direction = normalize(direction);

    velocity.x = direction.x * 250.f;
    velocity.y = direction.y * 250.f;
}

void FurnitureProjectile::update(float deltaTime) {
    sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);
}

void FurnitureProjectile::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool FurnitureProjectile::isOutside(sf::RenderWindow& window) {
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u size = window.getSize();

    return pos.x < 0.f ||
           pos.y < 0.f ||
           pos.x > size.x ||
           pos.y > size.y;
}

bool FurnitureProjectile::hitsPlayer(Player& player) {
    return sprite.getGlobalBounds().intersects(player.getBounds());
}
