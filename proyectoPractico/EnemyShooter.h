#ifndef ENEMYSHOOTER_H_INCLUDED
#define ENEMYSHOOTER_H_INCLUDED

#pragma once
#include <vector>
#include "EnemyBase.h"
#include "Projectile.h"
#include "Player.h"
#include "circle.h"
#include "TileMap.h"

// Estados del shooter.
// Permiten que el enemigo se reposicione, cargue y dispare en rafaga.
enum ShooterState {
    Repositioning,  // Mantiene distancia con el jugador.
    ChargingShot,  // Prepara el ataque.
    BurstShooting, // Dispara varios proyectiles.
    ShotRecovering // Descanso despues de disparar.
};

// Enemigo que pelea a distancia.
// Mantiene una distancia deseada y dispara proyectiles.
class EnemyShooter : public EnemyBase {
private:
    sf::Sprite sprite;

    // Textura compartida entre shooters.
    static sf::Texture shooterTexture;
    static bool shooterTextureLoaded;

    // Proyectiles activos disparados por este enemigo.
    std::vector<Projectile> projectiles;

    // Estado actual de su IA.
    ShooterState state;

    // Distancia que intenta mantener respecto al jugador.
    float desiredDistance;

    // Timers para disparos.
    float shootTimer;
    float shootCooldown;

    // Duraciones de carga y recuperacion.
    float chargeDuration;
    float recoverDuration;

    // Control de rafagas.
    int burstShotsLeft;
    float burstShotTimer;
    float burstShotDelay;
    int burstShotIndex;

    // Ultima direccion usada para disparar.
    sf::Vector2f lastShotDirection;

    // Rota una direccion para poder disparar en abanico.
    sf::Vector2f rotateVector(sf::Vector2f vector, float degrees);

    // Da vuelta el sprite segun la posicion del jugador.
    void updateFacing(sf::Vector2f direction);

    // Actualiza proyectiles: movimiento, colisiones y borrado.
    void updateProjectiles(
        float deltaTime,
        Player& player,
        circle& aspiradora,
        sf::RenderWindow& window,
        TileMap& tileMap
    );

protected:
    void syncSpritePosition();

public:
    EnemyShooter(sf::Vector2f position);

    // Actualiza movimiento, estados, disparos y proyectiles.
    void update(
        float deltaTime,
        Player& player,
        circle& aspiradora,
        sf::RenderWindow& window,
        TileMap& tileMap
    );

    void draw(sf::RenderWindow& window);
};

#endif
