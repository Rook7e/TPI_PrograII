#include "EnemyChaser.h"
#include "VectorMath.h"
#include <iostream>
#include <cmath>

// Textura compartida por todos los chasers.
// Al ser static, se carga una sola vez para todos los enemigos de este tipo.
sf::Texture EnemyChaser::texture;
bool EnemyChaser::textureLoaded = false;

EnemyChaser::EnemyChaser(sf::Vector2f position)
    : EnemyBase(position, 22.f, 120.f, 5)
{
    // Cargamos el sprite una sola vez.
    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Entidades/chasser.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Entidades/chasser.png" << std::endl;
        }
    }

    // Si cargo la textura, configuramos el sprite.
    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();
        float scale = 44.f / textureSize.x;

        sprite.setScale(scale, scale);
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        sprite.setPosition(position);

        // La hitbox queda invisible porque se ve el sprite.
        hitbox.setFillColor(sf::Color::Transparent);
    } else {
        // Fallback si no carga la textura.
        hitbox.setFillColor(sf::Color::Red);
    }

    // Cooldown para que no haga dano todos los frames al tocar al jugador.
    damageTimer = 0.f;
    damageCooldown = 0.8f;

    // Estado inicial de la IA.
    state = Chasing;
    dashDirection = sf::Vector2f(0.f, 0.f);

    // Timers del comportamiento.
    stateTimer = 0.f;
    dashCooldownTimer = 0.f;

    // Configuracion del dash.
    dashSpeed = 340.f;
    chargeDuration = 0.35f;
    dashDuration = 0.28f;
    recoverDuration = 0.45f;
    dashCooldown = 2.2f;
}

void EnemyChaser::syncSpritePosition() {
    // Sincroniza sprite con hitbox.
    sprite.setPosition(hitbox.getPosition());
}

void EnemyChaser::updateFacing(sf::Vector2f direction) {
    if (!textureLoaded) {
        return;
    }

    // Si el jugador esta a la derecha, mira a la derecha.
    if (direction.x > 0.f) {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    }
    // Si esta a la izquierda, espeja el sprite.
    else if (direction.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }
}

void EnemyChaser::update(float deltaTime, Player& player, sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    // Avanzan los timers.
    damageTimer += deltaTime;
    dashCooldownTimer += deltaTime;
    stateTimer += deltaTime;

    // Direccion desde enemigo hacia jugador.
    sf::Vector2f playerCenter = player.getCenter();
    sf::Vector2f rawDirection = playerCenter - hitbox.getPosition();
    sf::Vector2f direction = normalize(rawDirection);
    float distance = vectorLength(rawDirection);

    // El dash se activa si esta alineado horizontal o verticalmente.
    bool horizontallyAligned = std::abs(rawDirection.y) < 45.f;
    bool verticallyAligned = std::abs(rawDirection.x) < 45.f;

    bool canDash =
        dashCooldownTimer >= dashCooldown &&
        distance < 260.f &&
        (horizontallyAligned || verticallyAligned);

    if (state == Chasing) {
        // Persigue al jugador.
        tryMove(
            sf::Vector2f(direction.x * speed * deltaTime,
                         direction.y * speed * deltaTime),
            window
        );

        // Si puede dashear, pasa al estado de carga.
        if (canDash) {
            state = ChargingDash;
            stateTimer = 0.f;
            dashDirection = direction;
        }
    } else if (state == ChargingDash) {
        // Cambia color para avisar que va a atacar.
        if (textureLoaded) {
            sprite.setColor(sf::Color(255, 120, 120));
        }

        if (stateTimer >= chargeDuration) {
            state = Dashing;
            stateTimer = 0.f;
        }
    } else if (state == Dashing) {
        // Embestida rapida.
        tryMove(
            sf::Vector2f(dashDirection.x * dashSpeed * deltaTime,
                         dashDirection.y * dashSpeed * deltaTime),
            window
        );

        if (stateTimer >= dashDuration) {
            state = Recovering;
            stateTimer = 0.f;
        }
    } else if (state == Recovering) {
        // Tiempo de recuperacion despues del dash.
        if (textureLoaded) {
            sprite.setColor(sf::Color(180, 180, 180));
        }

        if (stateTimer >= recoverDuration) {
            state = Chasing;
            stateTimer = 0.f;
            dashCooldownTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color::White);
            }
        }
    }

    updateFacing(rawDirection);

    // Dano por contacto con cooldown.
    if (hitbox.getGlobalBounds().intersects(player.getBounds()) &&
        damageTimer >= damageCooldown) {
        player.takeDamage(1);
        damageTimer = 0.f;
    }
}

void EnemyChaser::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(hitbox);
    }

    // Barra de vida heredada de EnemyBase.
    drawHealthBar(window, 44.f);
}
