#include "Medkit.h"

Medkit::Medkit(sf::Vector2f position) {
    box.setSize(sf::Vector2f(24.f, 24.f));
    box.setFillColor(sf::Color::Red);
    box.setOrigin(12.f, 12.f);
    box.setPosition(position);

    crossVertical.setSize(sf::Vector2f(6.f, 18.f));
    crossVertical.setFillColor(sf::Color::White);
    crossVertical.setOrigin(3.f, 9.f);
    crossVertical.setPosition(position);

    crossHorizontal.setSize(sf::Vector2f(18.f, 6.f));
    crossHorizontal.setFillColor(sf::Color::White);
    crossHorizontal.setOrigin(9.f, 3.f);
    crossHorizontal.setPosition(position);
}

void Medkit::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(crossVertical);
    window.draw(crossHorizontal);
}

sf::FloatRect Medkit::getBounds() {
    return box.getGlobalBounds();
}
