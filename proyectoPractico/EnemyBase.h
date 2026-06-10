#ifndef ENEMYBASE_H_INCLUDED
#define ENEMYBASE_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

class EnemyBase {
protected:
    sf::CircleShape hitbox;

    float speed;
    int vida;
    int maxVida;

    virtual void syncSpritePosition();
    void drawHealthBar(sf::RenderWindow& window, float width);

public:
    EnemyBase(sf::Vector2f position, float radius, float speed, int maxVida);
    virtual ~EnemyBase();

    virtual void draw(sf::RenderWindow& window) = 0;

    void takeDamage(int damage, sf::Vector2f hitPosition);
    bool isDead();
    sf::FloatRect getBounds();
    sf::Vector2f getPosition();
};

#endif
