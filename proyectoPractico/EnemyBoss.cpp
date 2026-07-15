#include "EnemyBoss.h"
#include "VectorMath.h"
#include <algorithm>
#include <iostream>
#include <cmath>

// Textura compartida por todos los bosses.
// Como es static, se carga una sola vez aunque haya varios jefes.
sf::Texture EnemyBoss::texture;
bool EnemyBoss::textureLoaded = false;

EnemyBoss::EnemyBoss(sf::Vector2f position)
    : EnemyBase(position, 36.f, 95.f, 25)
{
    // Cargamos la textura del boss una sola vez.
    if (!textureLoaded) {
        if (texture.loadFromFile("assets/Entidades/boss.png")) {
            textureLoaded = true;
        } else {
            std::cout << "Error al cargar assets/Entidades/boss.png" << std::endl;
        }
    }

    if (textureLoaded) {
        sprite.setTexture(texture);

        sf::Vector2u textureSize = texture.getSize();
        float scale = 144.f / textureSize.x;

        // Centramos el sprite para que rote/espeje desde el centro.
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        sprite.setScale(scale, scale);
        sprite.setPosition(position);

        // La hitbox queda invisible porque usamos sprite.
        hitbox.setFillColor(sf::Color::Transparent);
    } else {
        // Fallback visual si no carga la textura.
        hitbox.setFillColor(sf::Color(120, 40, 160));
    }

    // Dano por contacto.
    damageTimer = 0.f;
    damageCooldown = 0.7f;

    // Dash lateral.
    dashTimer = 0.f;
    dashCooldown = 2.4f;
    dashDuration = 0.50f;
    dashSpeed = 500.f;
    dashing = false;
    dashDirection = sf::Vector2f(0.f, 0.f);

    // Disparos.
    shootTimer = 0.f;
    shootCooldown = 1.4f;
    burstShotsLeft = 0;
    burstShotTimer = 0.f;
    burstShotDelay = 0.04f;
    lastShotDirection = sf::Vector2f(0.f, 0.f);

    // Lanzamiento de muebles.
    throwTimer = 0.f;
    throwCooldown = 4.f;

    // Estado inicial del boss.
    state = BossMoving;
    stateTimer = 0.f;

    // Duraciones de carga y recuperacion.
    chargeDuration = 0.5f;
    recoverDuration = 0.7f;

    // Abanico de disparos.
    fanShotCount = 25;
    fanShotIndex = 0;
    fanAngle = 100.f;

    // Fase 2 empieza desactivada.
    phaseTwo = false;

    // Configuracion de dashes agresivos de fase 2.
    wallDashCount = 0;
    maxWallDashes = 3;

    wallDashChargeDuration = 0.35f;
    wallDashMaxDuration = 1.2f;
    wallDashSpeed = 620.f;

    wallDashDirection = sf::Vector2f(0.f, 0.f);
}

void EnemyBoss::syncSpritePosition() {
    // Mantiene sprite y hitbox en la misma posicion.
    sprite.setPosition(hitbox.getPosition());
}

void EnemyBoss::updateFacing(sf::Vector2f direction) {
    if (!textureLoaded) {
        return;
    }

    // Si el jugador esta a la derecha, mira a la derecha.
    if (direction.x > 0.f) {
        sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
    }
    // Si el jugador esta a la izquierda, espeja el sprite.
    else if (direction.x < 0.f) {
        sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
    }
}

sf::Vector2f EnemyBoss::rotateVector(sf::Vector2f vector, float degrees) {
    // Convierte grados a radianes para usar seno/coseno.
    float radians = degrees * 3.14159265f / 180.f;

    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);

    // Formula de rotacion de vector 2D.
    return sf::Vector2f(
        vector.x * cosAngle - vector.y * sinAngle,
        vector.x * sinAngle + vector.y * cosAngle
    );
}

void EnemyBoss::update(
    float deltaTime,
    Player& player,
    circle& aspiradora,
    sf::RenderWindow& window,
    TileMap& tileMap
) {
    if (isDead()) {
        return;
    }

    // Timers generales del boss.
    damageTimer += deltaTime;
    dashTimer += deltaTime;
    shootTimer += deltaTime;
    throwTimer += deltaTime;
    stateTimer += deltaTime;

    // Direccion hacia el jugador.
    sf::Vector2f directionToPlayer = player.getCenter() - hitbox.getPosition();
    sf::Vector2f direction = normalize(directionToPlayer);
    float distance = vectorLength(directionToPlayer);

    // Cambio de fase al llegar a 50% de vida.
    if (!phaseTwo && getLifePercent() <= 0.5f) {
        phaseTwo = true;

        // En fase 2 ataca mas seguido y con abanico mas amplio.
        dashCooldown = 1.4f;
        shootCooldown = 0.9f;
        fanShotCount = 28;
        fanAngle = 135.f;

        // Apenas entra en fase 2, empieza el patron de dash fuerte.
        wallDashCount = 0;
        startWallDash(player.getCenter());

        return;
    }

    if (state == BossMoving) {
        // Movimiento normal: se acerca hasta cierta distancia.
        if (distance > 180.f) {
            sf::Vector2f movement(
                direction.x * speed * deltaTime,
                direction.y * speed * deltaTime
            );

            tryMove(movement, window);
        }

        // Si paso el cooldown y ve al jugador, decide ataque.
        if (dashTimer >= dashCooldown &&
            tileMap.hasLineOfSight(hitbox.getPosition(), player.getCenter())) {

            dashTimer = 0.f;
            stateTimer = 0.f;

            // En fase 2 a veces usa dash fuerte hacia el jugador.
            if (phaseTwo && std::rand() % 2 == 0) {
                wallDashCount = 0;
                startWallDash(player.getCenter());
            } else {
                // Dash lateral: perpendicular a la direccion hacia el jugador.
                sf::Vector2f sideDirection(-direction.y, direction.x);

                // A veces invierte el lado.
                if (std::rand() % 2 == 0) {
                    sideDirection.x *= -1.f;
                    sideDirection.y *= -1.f;
                }

                dashDirection = sideDirection;
                state = BossSideDashing;

                if (textureLoaded) {
                    sprite.setColor(sf::Color(255, 160, 120));
                }
            }
        }
    } else if (state == BossSideDashing) {
        // Se reposiciona hacia un costado antes de disparar.
        sf::Vector2f movement(
            dashDirection.x * dashSpeed * deltaTime,
            dashDirection.y * dashSpeed * deltaTime
        );

        bool moved = tryMove(movement, window);

        // Cuando termina el dash, carga disparo.
        if (!moved || stateTimer >= dashDuration) {
            state = BossChargingShot;
            stateTimer = 0.f;
            lastShotDirection = player.getCenter() - hitbox.getPosition();

            if (textureLoaded) {
                sprite.setColor(sf::Color(140, 220, 255));
            }
        }
    } else if (state == BossWallDashCharging) {
        // Carga el dash fuerte mirando al jugador.
        wallDashDirection = normalize(player.getCenter() - hitbox.getPosition());
        updateFacing(wallDashDirection);

        if (stateTimer >= wallDashChargeDuration) {
            state = BossWallDashing;
            stateTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color(255, 255, 255));
            }
        }
    } else if (state == BossWallDashing) {
        // Dash fuerte de fase 2.
        sf::Vector2f movement(
            wallDashDirection.x * wallDashSpeed * deltaTime,
            wallDashDirection.y * wallDashSpeed * deltaTime
        );

        bool moved = tryMove(movement, window);

        // Repite varios dashes y luego descansa.
        if (!moved || stateTimer >= wallDashMaxDuration) {
            wallDashCount++;

            if (wallDashCount < maxWallDashes) {
                startWallDash(player.getCenter());
            } else {
                state = BossRecovering;
                stateTimer = 0.f;

                if (textureLoaded) {
                    sprite.setColor(sf::Color(180, 180, 180));
                }
            }
        }
    } else if (state == BossChargingShot) {
        // Mientras carga, actualiza direccion para apuntar mejor.
        lastShotDirection = player.getCenter() - hitbox.getPosition();

        if (stateTimer >= chargeDuration) {
            state = BossBurstShooting;
            stateTimer = 0.f;
            fanShotIndex = 0;
            burstShotTimer = burstShotDelay;

            if (textureLoaded) {
                sprite.setColor(sf::Color(255, 255, 255));
            }
        }
    } else if (state == BossBurstShooting) {
        burstShotTimer += deltaTime;

        // Dispara proyectiles distribuidos en un abanico.
        if (fanShotIndex < fanShotCount && burstShotTimer >= burstShotDelay) {
            float startAngle = -fanAngle / 2.f;
            float angleStep = fanAngle / (fanShotCount - 1);
            float angle = startAngle + angleStep * fanShotIndex;

            sf::Vector2f shotDirection = rotateVector(lastShotDirection, angle);

            projectiles.push_back(
                Projectile(hitbox.getPosition(), shotDirection)
            );

            fanShotIndex++;
            burstShotTimer = 0.f;
        }

        // Cuando termina el abanico, pasa a recuperacion.
        if (fanShotIndex >= fanShotCount) {
            state = BossRecovering;
            stateTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color(180, 180, 180));
            }
        }
    } else if (state == BossRecovering) {
        // Descanso entre ataques.
        if (stateTimer >= recoverDuration) {
            state = BossMoving;
            stateTimer = 0.f;

            if (textureLoaded) {
                sprite.setColor(sf::Color::White);
            }
        }
    }

    // Si esta en movimiento normal, tambien puede lanzar muebles.
    if (throwTimer >= throwCooldown && state == BossMoving) {
        sf::Vector2i furnitureTile;

        if (tileMap.findNearestFurniture(hitbox.getPosition(), furnitureTile)) {
            int furnitureId = tileMap.getFurnitureId(furnitureTile.x, furnitureTile.y);

            furnitureProjectiles.push_back(
                FurnitureProjectile(
                    tileMap.getBuildingTileset(),
                    hitbox.getPosition(),
                    player.getCenter() - hitbox.getPosition(),
                    furnitureId
                )
            );

            // El mueble se elimina del mapa porque el boss lo usa como proyectil.
            tileMap.removeFurniture(furnitureTile.x, furnitureTile.y);
            throwTimer = 0.f;
        }
    }

    syncSpritePosition();
    updateFacing(directionToPlayer);

    // Dano por contacto con el jugador.
    if (hitbox.getGlobalBounds().intersects(player.getBounds()) &&
        damageTimer >= damageCooldown) {
        player.takeDamage(1);
        damageTimer = 0.f;
    }

    // Actualiza proyectiles del boss.
    updateProjectiles(deltaTime, player, aspiradora, window, tileMap);
    updateFurnitureProjectiles(deltaTime, player, aspiradora, window, tileMap);
}

void EnemyBoss::updateProjectiles(
    float deltaTime,
    Player& player,
    circle& aspiradora,
    sf::RenderWindow& window,
    TileMap& tileMap
) {
    // El parametro tileMap queda por compatibilidad, pero los proyectiles normales
    // del boss atraviesan paredes, por eso no chequeamos colision con mapa.
    (void)tileMap;

    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].update(deltaTime);
    }

    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [&window, &player, &aspiradora](Projectile& projectile) {
                // La aspiradora puede bloquear proyectiles.
                if (projectile.shape.getGlobalBounds().intersects(aspiradora.getBounds())) {
                    return true;
                }

                // Si toca al jugador, hace dano y desaparece.
                if (projectile.hitsPlayer(player)) {
                    player.takeDamage(1);
                    return true;
                }

                // Si sale de la pantalla, se borra.
                return projectile.isOutside(window);
            }
        ),
        projectiles.end()
    );
}

void EnemyBoss::updateFurnitureProjectiles(
    float deltaTime,
    Player& player,
    circle& aspiradora,
    sf::RenderWindow& window,
    TileMap& tileMap
) {
    for (int i = 0; i < furnitureProjectiles.size(); i++) {
        furnitureProjectiles[i].update(deltaTime);
    }

    furnitureProjectiles.erase(
        std::remove_if(
            furnitureProjectiles.begin(),
            furnitureProjectiles.end(),
            [&window, &player, &aspiradora, &tileMap](FurnitureProjectile& projectile) {
                // Los muebles lanzados si chocan contra paredes.
                if (tileMap.checkCollision(projectile.sprite.getGlobalBounds())) {
                    return true;
                }

                // La aspiradora tambien puede bloquear muebles.
                if (projectile.sprite.getGlobalBounds().intersects(aspiradora.getBounds())) {
                    return true;
                }

                // Si impacta al jugador, hace dano.
                if (projectile.hitsPlayer(player)) {
                    player.takeDamage(1);
                    return true;
                }

                return projectile.isOutside(window);
            }
        ),
        furnitureProjectiles.end()
    );
}

void EnemyBoss::draw(sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    // Dibuja sprite o fallback.
    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(hitbox);
    }

    // Barra de vida local arriba del boss.
    drawHealthBar(window, 100.f);

    // Dibuja proyectiles normales.
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].draw(window);
    }

    // Dibuja muebles lanzados.
    for (int i = 0; i < furnitureProjectiles.size(); i++) {
        furnitureProjectiles[i].draw(window);
    }
}

float EnemyBoss::getLifePercent() {
    if (maxVida <= 0) {
        return 0.f;
    }

    return (float)vida / maxVida;
}

void EnemyBoss::startWallDash(sf::Vector2f targetPosition) {
    // Calcula direccion hacia el objetivo del dash.
    wallDashDirection = normalize(targetPosition - hitbox.getPosition());

    // Evita direccion cero.
    if (vectorLength(wallDashDirection) == 0.f) {
        wallDashDirection = sf::Vector2f(1.f, 0.f);
    }

    // Pasa al estado de carga del dash fuerte.
    state = BossWallDashCharging;
    stateTimer = 0.f;

    updateFacing(wallDashDirection);

    if (textureLoaded) {
        sprite.setColor(sf::Color(255, 80, 80));
    }
}
