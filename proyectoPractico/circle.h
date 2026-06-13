#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include "MangueraAspiradora.h"

class circle {
private:
    sf::CircleShape shape;
    sf::Vector2f attachPoint;
    float maxDistance;

    sf::Sprite sprite;
    sf::Texture texture;
    bool textureLoaded;

    MangueraAspiradora manguera;

public:
    circle();

    void update(sf::RenderWindow& window, sf::Vector2f playerCenter);
    void draw(sf::RenderWindow& window);
    void setMaxDistance(float distance);
    sf::FloatRect getBounds();
    sf::Vector2f getPosition();

};

#endif // CIRCLE_H_INCLUDED
