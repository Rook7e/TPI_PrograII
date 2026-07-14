#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

// Clase que representa al jugador.
// Maneja sprite, movimiento, animacion, vida, stamina y colisiones.
class Player {
private:
    // Textura y sprite visual del jugador.
    sf::Texture texture;
    sf::Sprite sprite;

    // Variables para animacion por frames.
    int currentFrame;
    float animationTimer;

    // Velocidad actual y estados de movimiento.
    float speed;
    bool running;
    bool moving;

    // Velocidades base: caminar y correr.
    float walkSpeed;
    float runSpeed;

    // Sistema de stamina para correr.
    float stamina;
    float maxStamina;
    float staminaDrain;
    float staminaRegen;
    float staminaRegenDelay;
    float staminaRegenTimer;

    // Vida del jugador.
    int vida;
    int maxVida;

    // Barra visual de vida.
    sf::RectangleShape healthBack;
    sf::RectangleShape healthBar;

public:
    Player();

    // Lee teclado y mueve al jugador.
    void movement();

    // Actualiza animacion, stamina y estado general.
    void update(float deltaTime);

    // Dibuja el sprite del jugador.
    void draw(sf::RenderWindow& window);

    // Dibuja la barra de stamina.
    void drawStamina(sf::RenderWindow& window);

    // Devuelve el centro del jugador, util para enemigos y aspiradora.
    sf::Vector2f getCenter();

    // Barra visual de stamina.
    sf::RectangleShape staminaBack;
    sf::RectangleShape staminaBar;

    // Vida y curacion.
    int getVida();
    void heal(int amount);
    void reset();
    void drawLife(sf::RenderWindow& window);
    void takeDamage(int damage);
    bool isDead();

    // Hitbox del jugador para colisiones.
    sf::FloatRect getBounds();

    // Posicion del jugador.
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);
};

#endif
