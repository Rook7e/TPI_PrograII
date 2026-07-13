#include "MangueraAspiradora.h"
#include <cmath>

MangueraAspiradora::MangueraAspiradora() {
    // Cantidad de puntos que forman la manguera.
    pointCount = 14;

    // Largo inicial aproximado dividido en segmentos.
    segmentLength = 120.f / (pointCount - 1);

    // Valores simples para simular peso e inercia.
    gravity = 0.35f;
    damping = 0.92f;

    initialized = false;
}

void MangueraAspiradora::setTotalLength(float totalLength) {
    // Cuando cambia el rango de la aspiradora,
    // recalculamos el largo de cada segmento.
    segmentLength = totalLength / (pointCount - 1);

    // Forzamos reinicializacion para evitar formas raras.
    initialized = false;
}

float MangueraAspiradora::getLength(sf::Vector2f vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

void MangueraAspiradora::initialize(sf::Vector2f start, sf::Vector2f end) {
    points.clear();
    previousPoints.clear();

    sf::Vector2f direction = end - start;

    // Creamos puntos interpolados entre inicio y fin.
    for (int i = 0; i < pointCount; i++) {
        float t = (float)i / (pointCount - 1);
        sf::Vector2f point = start + direction * t;

        points.push_back(point);
        previousPoints.push_back(point);
    }

    initialized = true;
}

void MangueraAspiradora::update(sf::Vector2f start, sf::Vector2f end) {
    if (!initialized) {
        initialize(start, end);
    }

    // Primer punto pegado al jugador y ultimo punto pegado a la punta.
    points[0] = start;
    points[pointCount - 1] = end;

    previousPoints[0] = start;
    previousPoints[pointCount - 1] = end;

    // Simulacion tipo Verlet: usa posicion actual y anterior para generar movimiento.
    for (int i = 1; i < pointCount - 1; i++) {
        sf::Vector2f velocity = (points[i] - previousPoints[i]) * damping;

        previousPoints[i] = points[i];
        points[i] += velocity;
        points[i].y += gravity;
    }

    // Repetimos constraints varias veces para mantener distancia entre puntos.
    for (int i = 0; i < 8; i++) {
        solveConstraints(start, end);
    }
}

void MangueraAspiradora::solveConstraints(sf::Vector2f start, sf::Vector2f end) {
    points[0] = start;
    points[pointCount - 1] = end;

    for (int i = 0; i < pointCount - 1; i++) {
        sf::Vector2f delta = points[i + 1] - points[i];
        float distance = getLength(delta);

        if (distance == 0.f) {
            continue;
        }

        // Calcula cuanto se estiro o achico el segmento.
        float difference = (distance - segmentLength) / distance;
        sf::Vector2f correction = delta * difference;

        // Los extremos quedan fijos, los puntos del medio se corrigen.
        if (i == 0) {
            points[i + 1] -= correction;
        } else if (i == pointCount - 2) {
            points[i] += correction;
        } else {
            points[i] += correction * 0.5f;
            points[i + 1] -= correction * 0.5f;
        }
    }

    points[0] = start;
    points[pointCount - 1] = end;
}

void MangueraAspiradora::draw(sf::RenderWindow& window) {
    // Dibujamos cada tramo de la manguera como un rectangulo rotado.
    for (int i = 0; i < pointCount - 1; i++) {
        sf::Vector2f a = points[i];
        sf::Vector2f b = points[i + 1];
        sf::Vector2f segment = b - a;

        float length = getLength(segment);
        float angle = std::atan2(segment.y, segment.x) * 180.f / 3.14159265f;

        sf::RectangleShape hosePart(sf::Vector2f(length + 2.f, 8.f));
        hosePart.setOrigin(0.f, 4.f);
        hosePart.setPosition(a);
        hosePart.setRotation(angle);
        hosePart.setFillColor(sf::Color(55, 55, 65));
        window.draw(hosePart);

        // Brillo superior para dar volumen.
        sf::RectangleShape highlight(sf::Vector2f(length + 2.f, 2.f));
        highlight.setOrigin(0.f, 1.f);
        highlight.setPosition(a);
        highlight.setRotation(angle);
        highlight.setFillColor(sf::Color(115, 115, 130));
        window.draw(highlight);
    }
}
