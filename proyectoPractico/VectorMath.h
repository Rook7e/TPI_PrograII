#ifndef VECTORMATH_H_INCLUDED
#define VECTORMATH_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

inline float vectorLength(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline sf::Vector2f normalize(sf::Vector2f vector) {
    float length = vectorLength(vector);

    if (length == 0.f) {
        return sf::Vector2f(0.f, 0.f);
    }

    return sf::Vector2f(vector.x / length, vector.y / length);
}

#endif
