#include "circle.h"
#include <cmath>

circle::circle() {
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(20.f, 20.f);

    maxDistance = 120.f;
    attachPoint = sf::Vector2f(0.f, 0.f);

    textureLoaded = texture.loadFromFile("assets/Extras/puntaAspiradora.png");

    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

        float scale = 40.f / textureSize.x;
        sprite.setScale(scale, scale);
    } else {
        shape.setFillColor(sf::Color::Cyan);
    }
}

void circle::update(sf::RenderWindow& window, sf::Vector2f playerCenter) {
    attachPoint = playerCenter;

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    sf::Vector2f direction = mouseWorld - playerCenter;

    float distance = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (distance > maxDistance) {
        direction.x /= distance;
        direction.y /= distance;

        shape.setPosition(
            playerCenter.x + direction.x * maxDistance,
            playerCenter.y + direction.y * maxDistance
        );
    } else {
        shape.setPosition(mouseWorld);
    }

    sprite.setPosition(shape.getPosition());

    sf::Vector2f aimDirection = shape.getPosition() - playerCenter;
    float angle = std::atan2(aimDirection.y, aimDirection.x) * 180.f / 3.14159265f;

    sprite.setRotation(angle);

    manguera.update(playerCenter, shape.getPosition());
}

void circle::draw(sf::RenderWindow& window) {
    manguera.draw(window);

    sf::CircleShape connector(7.f);
    connector.setOrigin(7.f, 7.f);
    connector.setFillColor(sf::Color(45, 45, 52));

    connector.setPosition(attachPoint);
    window.draw(connector);

    connector.setPosition(shape.getPosition());
    window.draw(connector);

    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
}

sf::FloatRect circle::getBounds() {
    return shape.getGlobalBounds();
}

sf::Vector2f circle::getPosition() {
    return shape.getPosition();
}

void circle::setMaxDistance(float distance) {
    maxDistance = distance;
    manguera.setTotalLength(maxDistance);
}
