#ifndef ENEMYBOSS_H_INCLUDED
#define ENEMYBOSS_H_INCLUDED

#pragma once
#include <vector>
#include "EnemyBase.h"
#include "Projectile.h"
#include "FurnitureProjectile.h"
#include "Player.h"
#include "circle.h"
#include "TileMap.h"

// Estados del jefe.
// El boss combina movimiento, dash lateral, dash contra paredes, carga y disparos.
enum BossState {
    BossMoving,            // Movimiento normal.
    BossSideDashing,       // Dash lateral antes de disparar.
    BossWallDashCharging,  // Mira al jugador y prepara dash fuerte.
    BossWallDashing,       // Dash directo hasta el borde de la sala.
    BossChargingShot,      // Carga el ataque de proyectiles.
    BossBurstShooting,     // Dispara abanico/rafaga.
    BossRecovering         // Recuperacion despues de atacar.
};

// Jefe del piso.
// Hereda de EnemyBase y mezcla mecanicas de varios enemigos.
class EnemyBoss : public EnemyBase {
private:
    sf::Sprite sprite;

    // Textura compartida entre bosses.
    static sf::Texture texture;
    static bool textureLoaded;

    // Proyectiles normales y proyectiles de muebles.
    std::vector<Projectile> projectiles;
    std::vector<FurnitureProjectile> furnitureProjectiles;

    // Dano por contacto.
    float damageTimer;
    float damageCooldown;

    // Dash lateral.
    float dashTimer;
    float dashCooldown;
    float dashDuration;
    float dashSpeed;
    bool dashing;
    sf::Vector2f dashDirection;

    // Disparos.
    float shootTimer;
    float shootCooldown;
    int burstShotsLeft;
    float burstShotTimer;
    float burstShotDelay;
    sf::Vector2f lastShotDirection;

    // Estado general del boss.
    BossState state;
    float stateTimer;

    // Duracion de carga y recuperacion.
    float chargeDuration;
    float recoverDuration;

    // Configuracion del disparo en abanico.
    int fanShotCount;
    int fanShotIndex;
    float fanAngle;

    // Lanzamiento de muebles.
    float throwTimer;
    float throwCooldown;

    // Fase 2.
    bool phaseTwo;

    // Dashes agresivos de fase 2.
    int wallDashCount;
    int maxWallDashes;
    float wallDashChargeDuration;
    float wallDashMaxDuration;
    float wallDashSpeed;
    sf::Vector2f wallDashDirection;

    // Inicia un dash directo hacia el jugador.
    void startWallDash(sf::Vector2f targetPosition);

    // Actualiza proyectiles normales.
    void updateProjectiles(
        float deltaTime,
        Player& player,
        circle& aspiradora,
        sf::RenderWindow& window,
        TileMap& tileMap
    );

    // Actualiza proyectiles de muebles.
    void updateFurnitureProjectiles(
        float deltaTime,
        Player& player,
        circle& aspiradora,
        sf::RenderWindow& window,
        TileMap& tileMap
    );

    // Rota una direccion para armar abanicos de disparo.
    sf::Vector2f rotateVector(sf::Vector2f vector, float degrees);

    // Voltea el sprite mirando al jugador.
    void updateFacing(sf::Vector2f direction);

protected:
    void syncSpritePosition();

public:
    EnemyBoss(sf::Vector2f position);

    // Devuelve porcentaje de vida para la barra superior.
    float getLifePercent();

    // IA completa del boss.
    void update(
        float deltaTime,
        Player& player,
        circle& aspiradora,
        sf::RenderWindow& window,
        TileMap& tileMap
    );

    // Dibuja boss, barra local y proyectiles.
    void draw(sf::RenderWindow& window);
};

#endif
