#ifndef ENEMYTHROWER_H_INCLUDED
#define ENEMYTHROWER_H_INCLUDED

#pragma once
#include <vector>
#include "EnemyBase.h"
#include "FurnitureProjectile.h"
#include "Player.h"
#include "circle.h"
#include "TileMap.h"

// Enemigo que usa el escenario.
// Busca muebles en el mapa, los toma y los lanza al jugador.
class EnemyThrower : public EnemyBase {
private:
    sf::Sprite sprite;

    // Textura compartida entre throwers.
    static sf::Texture texture;
    static bool textureLoaded;

    // Proyectiles de muebles que lanza este enemigo.
    std::vector<FurnitureProjectile> furnitureProjectiles;

    // Timer para controlar cada cuanto lanza.
    float throwTimer;
    float throwCooldown;

    // Dano por contacto.
    float damageTimer;
    float damageCooldown;

protected:
    void syncSpritePosition();

public:
    EnemyThrower(sf::Vector2f position);

    // Actualiza busqueda de muebles, lanzamiento y proyectiles.
    void update(
        float deltaTime,
        Player& player,
        circle& aspiradora,
        sf::RenderWindow& window,
        TileMap& tileMap
    );

    void draw(sf::RenderWindow& window);

    // Datos del comportamiento de agarrar muebles.
    bool holdingFurniture;
    int carriedFurnitureId;
    bool targetingFurniture;
    sf::Vector2i targetFurnitureTile;
};

#endif
