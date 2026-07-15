#ifndef ENEMYBASE_H_INCLUDED
#define ENEMYBASE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

// Clase base para todos los enemigos.
// De aca heredan Chaser, Shooter, Thrower y Boss.
// Guarda lo comun: hitbox, vida, velocidad, dano recibido y barra de vida.
class EnemyBase {
protected:
    // Hitbox circular usada para colisiones y posicion.
    sf::CircleShape hitbox;

    // Stats basicos compartidos por todos los enemigos.
    float speed;
    int vida;
    int maxVida;

    // Cada enemigo redefine esto para mantener su sprite sincronizado con la hitbox.
    virtual void syncSpritePosition();

    // Dibuja la barra de vida encima del enemigo.
    void drawHealthBar(sf::RenderWindow& window, float width);

    // Movimiento comun que evita que el enemigo se vaya fuera de la ventana.
    bool tryMove(sf::Vector2f movement, sf::RenderWindow& window);
    void clampToWindow(sf::RenderWindow& window);

public:
    // Constructor base: posicion, radio de hitbox, velocidad y vida maxima.
    EnemyBase(sf::Vector2f position, float radius, float speed, int maxVida);

    // Destructor virtual porque esta clase se usa como base.
    virtual ~EnemyBase();

    // Metodo polimorfico: cada enemigo se dibuja distinto.
    virtual void draw(sf::RenderWindow& window) = 0;

    // Recibe dano y aplica knockback desde la posicion del golpe.
    void takeDamage(int damage, sf::Vector2f hitPosition, sf::RenderWindow& window);

    // Consultas comunes.
    bool isDead();
    sf::FloatRect getBounds();
    sf::Vector2f getPosition();

    // Escala vida y velocidad segun el piso actual.
    void applyDifficulty(float multiplier);
};

#endif
