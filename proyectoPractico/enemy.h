#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "circle.h"

struct Projectile {
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Projectile(sf::Vector2f position, sf::Vector2f direction);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isOutside(sf::RenderWindow& window);
    bool hitsPlayer(Player& player);
};

class EnemyChaser {
private:
    sf::CircleShape shape;
    float speed;
    int vida;
    int maxVida;
    float damageTimer;
    float damageCooldown;

public:
    EnemyChaser(sf::Vector2f position);

    void update(float deltaTime, Player& player);
    void draw(sf::RenderWindow& window);

    void takeDamage(int damage, sf::Vector2f hitPosition);
    bool isDead();
    sf::FloatRect getBounds();
};

class EnemyShooter {
private:
    sf::CircleShape shape;
    std::vector<Projectile> projectiles;

    float speed;
    float desiredDistance;
    float shootTimer;
    float shootCooldown;
    int vida;
    int maxVida;

public:
    EnemyShooter(sf::Vector2f position);

    void update(float deltaTime, Player& player, circle& aspiradora, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void takeDamage(int damage, sf::Vector2f hitPosition);
    bool isDead();
    sf::FloatRect getBounds();
};

#endif // ENEMY_H_INCLUDED
