#include "circle.h"
#include <cmath>

circle::circle() {
    // Hitbox circular de la punta de la aspiradora.
    // Aunque tenga sprite, esta forma se usa para colisiones.
    shape.setRadius(20.f);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(20.f, 20.f);

    // Distancia maxima inicial entre jugador y aspiradora.
    maxDistance = 120.f;

    // Punto donde se engancha la manguera al jugador.
    attachPoint = sf::Vector2f(0.f, 0.f);

    // Intentamos cargar el sprite de la punta.
    textureLoaded = texture.loadFromFile("assets/Extras/puntaAspiradora.png");

    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();

        // Centramos el origen para rotar desde el centro.
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

        // Escalamos para que mida aproximadamente 40 px de ancho.
        float scale = 40.f / textureSize.x;
        sprite.setScale(scale, scale);
    } else {
        // Si no carga textura, se muestra un circulo cyan como fallback.
        shape.setFillColor(sf::Color::Cyan);
    }
}

void circle::update(sf::RenderWindow& window, sf::Vector2f playerCenter) {
    // La manguera empieza desde el centro del jugador.
    attachPoint = playerCenter;

    // Obtenemos la posicion del mouse dentro de la ventana.
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    // Vector desde el jugador hasta el mouse.
    sf::Vector2f direction = mouseWorld - playerCenter;

    float distance = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    // Si el mouse esta mas lejos que el rango permitido,
    // normalizamos la direccion y limitamos la posicion.
    if (distance > maxDistance) {
        direction.x /= distance;
        direction.y /= distance;

        shape.setPosition(
            playerCenter.x + direction.x * maxDistance,
            playerCenter.y + direction.y * maxDistance
        );
    } else {
        // Si esta dentro del rango, la punta sigue exactamente al mouse.
        shape.setPosition(mouseWorld);
    }

    // El sprite sigue la posicion de la hitbox.
    sprite.setPosition(shape.getPosition());

    // Calculamos angulo para que la punta mire hacia donde apunta.
    sf::Vector2f aimDirection = shape.getPosition() - playerCenter;
    float angle = std::atan2(aimDirection.y, aimDirection.x) * 180.f / 3.14159265f;

    sprite.setRotation(angle);

    // Actualizamos la manguera entre jugador y punta.
    manguera.update(playerCenter, shape.getPosition());
}

void circle::draw(sf::RenderWindow& window) {
    // Primero dibujamos la manguera.
    manguera.draw(window);

    // Conectores visuales en ambos extremos.
    sf::CircleShape connector(7.f);
    connector.setOrigin(7.f, 7.f);
    connector.setFillColor(sf::Color(45, 45, 52));

    connector.setPosition(attachPoint);
    window.draw(connector);

    connector.setPosition(shape.getPosition());
    window.draw(connector);

    // Dibujamos sprite o fallback.
    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
}

sf::FloatRect circle::getBounds() {
    // Bounds usados para limpiar mugre, golpear enemigos y bloquear proyectiles.
    return shape.getGlobalBounds();
}

sf::Vector2f circle::getPosition() {
    return shape.getPosition();
}

void circle::setMaxDistance(float distance) {
    // Se llama cuando compramos mejora de rango.
    maxDistance = distance;

    // La manguera tambien necesita saber el nuevo largo.
    manguera.setTotalLength(maxDistance);
}
