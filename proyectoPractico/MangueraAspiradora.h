#ifndef MANGUERAASPIRADORA_H_INCLUDED
#define MANGUERAASPIRADORA_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Clase visual para simular la manguera de la aspiradora.
// Usa varios puntos conectados entre si para dar sensacion de cuerda/manguera.
class MangueraAspiradora {
private:
    // Puntos actuales de la manguera.
    std::vector<sf::Vector2f> points;

    // Posiciones anteriores, usadas para simular movimiento con inercia.
    std::vector<sf::Vector2f> previousPoints;

    // Cantidad de puntos que forman la manguera.
    int pointCount;

    // Longitud deseada entre cada punto.
    float segmentLength;

    // Simulacion simple de gravedad y amortiguacion.
    float gravity;
    float damping;

    // Indica si ya se inicializaron los puntos.
    bool initialized;

    // Helpers internos.
    float getLength(sf::Vector2f vector);
    void initialize(sf::Vector2f start, sf::Vector2f end);
    void solveConstraints(sf::Vector2f start, sf::Vector2f end);

public:
    MangueraAspiradora();

    // Actualiza la posicion de los puntos entre jugador y punta.
    void update(sf::Vector2f start, sf::Vector2f end);

    // Dibuja la manguera como varios segmentos rectangulares.
    void draw(sf::RenderWindow& window);

    // Cambia el largo total cuando mejora el rango de aspiradora.
    void setTotalLength(float totalLength);
};

#endif
