#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;

    int currentFrame;
    float animationTimer;
    float speed;
    bool running;
    bool moving;

    float walkSpeed;
    float runSpeed;
    float stamina;
    float maxStamina;
    float staminaDrain;
    float staminaRegen;
    float staminaRegenDelay;
    float staminaRegenTimer;

    int vida;
    int maxVida;
    sf::RectangleShape healthBack;
    sf::RectangleShape healthBar;

public:
    Player();

    void movement();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void drawStamina(sf::RenderWindow& window);

    sf::Vector2f getCenter();
    /*
    float stamina;
    float maxStamina;
*/

    sf::RectangleShape staminaBack;
    sf::RectangleShape staminaBar;

    int getVida();
    void heal(int amount);
    void reset();
    void drawLife(sf::RenderWindow& window);
    void takeDamage(int damage);
    bool isDead();
    sf::FloatRect getBounds();

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);
    };

#endif // PLAYER_H_INCLUDED
