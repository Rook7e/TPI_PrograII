#ifndef MANGUERAASPIRADORA_H_INCLUDED
#define MANGUERAASPIRADORA_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class MangueraAspiradora {
private:
    std::vector<sf::Vector2f> points;
    std::vector<sf::Vector2f> previousPoints;

    int pointCount;
    float segmentLength;
    float gravity;
    float damping;
    bool initialized;

    float getLength(sf::Vector2f vector);
    void initialize(sf::Vector2f start, sf::Vector2f end);
    void solveConstraints(sf::Vector2f start, sf::Vector2f end);

public:
    MangueraAspiradora();

    void update(sf::Vector2f start, sf::Vector2f end);
    void draw(sf::RenderWindow& window);
    void setTotalLength(float totalLength);
};

#endif
