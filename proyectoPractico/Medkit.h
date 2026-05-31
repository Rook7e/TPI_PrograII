#ifndef MEDKIT_H_INCLUDED
#define MEDKIT_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

class Medkit {
private:
    sf::RectangleShape box;
    sf::RectangleShape crossVertical;
    sf::RectangleShape crossHorizontal;

public:
    Medkit(sf::Vector2f position);

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
};

#endif
