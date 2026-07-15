#ifndef ENEMYCHASER_H_INCLUDED
#define ENEMYCHASER_H_INCLUDED

#pragma once
#include "EnemyBase.h"
#include "Player.h"

// Estados posibles del chaser.
// Sirven para que no sea solo "seguir al jugador", sino que tenga comportamiento.
enum ChaserState {
    Chasing,        // Persigue normalmente al jugador.
    ChargingDash,  // Se prepara para hacer dash.
    Dashing,        // Hace una embestida rapida.
    Recovering      // Pausa corta despues del dash.
};

// Enemigo que persigue al jugador y puede embestir.
// Hereda vida, velocidad, hitbox y dano desde EnemyBase.
class EnemyChaser : public EnemyBase {
private:
    // Sprite visual del enemigo.
    sf::Sprite sprite;

    // Textura compartida por todos los chasers.
    static sf::Texture texture;
    static bool textureLoaded;

    // Control de dano al tocar al jugador.
    float damageTimer;
    float damageCooldown;

    // Estado actual de la IA.
    ChaserState state;

    // Direccion usada durante el dash.
    sf::Vector2f dashDirection;

    // Timers del comportamiento.
    float stateTimer;
    float dashCooldownTimer;

    // Configuracion del dash.
    float dashSpeed;
    float chargeDuration;
    float dashDuration;
    float recoverDuration;
    float dashCooldown;

    // Da vuelta el sprite segun donde este el jugador.
    void updateFacing(sf::Vector2f direction);

protected:
    // Mantiene el sprite en la misma posicion que la hitbox.
    void syncSpritePosition();

public:
    EnemyChaser(sf::Vector2f position);

    // Actualiza persecucion, dash y dano al jugador.
    void update(float deltaTime, Player& player, sf::RenderWindow& window);

    // Dibuja sprite y barra de vida.
    void draw(sf::RenderWindow& window);
};

#endif
