#include "circle.h"
#include <cmath>

circle::circle() {
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(20.f, 20.f);

    maxDistance = 120.f;
    attachPoint = sf::Vector2f(0.f, 0.f);
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
}

void circle::draw(sf::RenderWindow& window) {
    sf::Vertex hose[] = {
        sf::Vertex(attachPoint, sf::Color::White),
        sf::Vertex(shape.getPosition(), sf::Color::White)
    };

    window.draw(hose, 2, sf::Lines);
    window.draw(shape);
}

sf::FloatRect circle::getBounds() {
    return shape.getGlobalBounds();
}

sf::Vector2f circle::getPosition() {
    return shape.getPosition();
}
