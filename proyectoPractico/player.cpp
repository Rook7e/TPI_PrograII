#include "Player.h"
#include <iostream>

Player::Player() {
    // Cargamos la textura del jugador.
    if (!texture.loadFromFile("assets/Entidades/pj_juego.png")) {
        std::cout << "Error al cargar assets/Entidades/pj_juego.png" << std::endl;
    }

    sprite.setTexture(texture);

    // Inicializamos todos los valores desde reset para reutilizarlo al reiniciar.
    reset();
}

void Player::reset() {
    // Reinicio de animacion.
    currentFrame = 0;
    animationTimer = 0.f;

    // Velocidades base.
    walkSpeed = 4.f;
    runSpeed = 6.f;
    speed = walkSpeed;

    // Sistema de stamina.
    maxStamina = 100.f;
    stamina = maxStamina;
    staminaDrain = 35.f;
    staminaRegen = 20.f;
    staminaRegenDelay = 2.f;
    staminaRegenTimer = 0.f;

    // Vida.
    maxVida = 5;
    vida = maxVida;

    moving = false;
    running = false;

    // Primer frame del sprite.
    sprite.setTextureRect(sf::IntRect(0, 0, 320, 320));

    // Escala y origen del personaje.
    sprite.setScale(0.2f, 0.2f);
    sprite.setOrigin(160.f, 0.f);
    sprite.setPosition(100.f, 100.f);

    // Barra de stamina.
    staminaBack.setSize(sf::Vector2f(100.f, 10.f));
    staminaBack.setFillColor(sf::Color(40, 40, 40));
    staminaBack.setPosition(20.f, 20.f);

    staminaBar.setSize(sf::Vector2f(100.f, 10.f));
    staminaBar.setFillColor(sf::Color::Green);
    staminaBar.setPosition(20.f, 20.f);

    // Barra de vida.
    healthBack.setSize(sf::Vector2f(100.f, 10.f));
    healthBack.setFillColor(sf::Color(80, 20, 20));
    healthBack.setPosition(20.f, 40.f);

    healthBar.setSize(sf::Vector2f(100.f, 10.f));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(20.f, 40.f);
}

void Player::movement() {
    moving = false;
    running = false;

    bool shiftPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

    // Solo corre si hay stamina.
    running = shiftPressed && stamina > 0.f;

    if (running) {
        speed = runSpeed;
    } else {
        speed = walkSpeed;
    }

    // Movimiento horizontal y flip del sprite.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sprite.move(speed, 0.f);
        sprite.setScale(0.2f, 0.2f);
        moving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sprite.move(-speed, 0.f);
        sprite.setScale(-0.2f, 0.2f);
        moving = true;
    }

    // Movimiento vertical.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        sprite.move(0.f, -speed);
        moving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sprite.move(0.f, speed);
        moving = true;
    }

    // Si no se mueve, no consume stamina corriendo.
    if (!moving) {
        running = false;
    }
}

void Player::update(float deltaTime) {
    bool shiftPressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

    // Consumo de stamina al correr y moverse.
    if (running && moving) {
        stamina -= staminaDrain * deltaTime;
        staminaRegenTimer = 0.f;

        if (stamina <= 0.f) {
            stamina = 0.f;
            running = false;
            speed = walkSpeed;
        }
    } else {
        // Si sigue apretando shift, no regenera.
        if (shiftPressed) {
            staminaRegenTimer = 0.f;
        } else {
            // Regenera despues de un delay.
            staminaRegenTimer += deltaTime;

            if (staminaRegenTimer >= staminaRegenDelay) {
                stamina += staminaRegen * deltaTime;

                if (stamina > maxStamina) {
                    stamina = maxStamina;
                }
            }
        }
    }

    // Actualiza barra visual de stamina.
    float staminaPercent = stamina / maxStamina;
    staminaBar.setSize(sf::Vector2f(100.f * staminaPercent, 10.f));

    // Animacion de caminata.
    if (moving) {
        animationTimer += deltaTime;

        if (animationTimer >= 0.15f) {
            currentFrame++;

            if (currentFrame >= 4) {
                currentFrame = 0;
            }

            sprite.setTextureRect(sf::IntRect(currentFrame * 320, 0, 320, 320));
            animationTimer = 0.f;
        }
    } else {
        // Si esta quieto vuelve al primer frame.
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect(0, 0, 320, 320));
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::drawStamina(sf::RenderWindow& window) {
    window.draw(staminaBack);
    window.draw(staminaBar);
}

void Player::drawLife(sf::RenderWindow& window) {
    window.draw(healthBack);
    window.draw(healthBar);
}

sf::Vector2f Player::getCenter() {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    return sf::Vector2f(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );
}

sf::FloatRect Player::getBounds() {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() {
    return sprite.getPosition();
}

void Player::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

void Player::takeDamage(int damage) {
    vida -= damage;

    if (vida < 0) {
        vida = 0;
    }

    float lifePercent = (float)vida / maxVida;
    healthBar.setSize(sf::Vector2f(100.f * lifePercent, 10.f));

    std::cout << "Vida player: " << vida << std::endl;
}

void Player::heal(int amount) {
    vida += amount;

    if (vida > maxVida) {
        vida = maxVida;
    }

    float lifePercent = (float)vida / maxVida;
    healthBar.setSize(sf::Vector2f(100.f * lifePercent, 10.f));
}

bool Player::isDead() {
    return vida <= 0;
}

int Player::getVida() {
    return vida;
}
