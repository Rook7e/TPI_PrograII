#include "EnemyBase.h"
#include "VectorMath.h"

EnemyBase::EnemyBase(sf::Vector2f position, float radius, float newSpeed, int newMaxVida) {
    // Hitbox circular comun a todos los enemigos.
    hitbox.setRadius(radius);
    hitbox.setOrigin(radius, radius);
    hitbox.setPosition(position);
    hitbox.setFillColor(sf::Color::Transparent);

    // Stats base.
    speed = newSpeed;
    maxVida = newMaxVida;
    vida = maxVida;
}

EnemyBase::~EnemyBase() {
}

void EnemyBase::syncSpritePosition() {
    // Funcion vacia en la clase base.
    // Cada enemigo la redefine para mover su sprite junto a la hitbox.
}

void EnemyBase::takeDamage(int damage, sf::Vector2f hitPosition, sf::RenderWindow& window) {
    if (isDead()) {
        return;
    }

    // Restamos vida.
    vida -= damage;

    // Calculamos direccion de knockback alejandose del punto de golpe.
    sf::Vector2f knockDirection = normalize(hitbox.getPosition() - hitPosition);

    hitbox.move(knockDirection.x * 45.f, knockDirection.y * 45.f);

    // Evita que el knockback saque al enemigo de la pantalla.
    clampToWindow(window);

    if (vida < 0) {
        vida = 0;
    }
}

bool EnemyBase::isDead() {
    return vida <= 0;
}

sf::FloatRect EnemyBase::getBounds() {
    return hitbox.getGlobalBounds();
}

sf::Vector2f EnemyBase::getPosition() {
    return hitbox.getPosition();
}

void EnemyBase::drawHealthBar(sf::RenderWindow& window, float width) {
    // Fondo de la barra.
    sf::RectangleShape back(sf::Vector2f(width, 6.f));
    back.setFillColor(sf::Color(70, 20, 20));
    back.setPosition(
        hitbox.getPosition().x - width / 2.f,
        hitbox.getPosition().y - hitbox.getRadius() - 14.f
    );

    // Porcentaje de vida restante.
    float percent = (float)vida / maxVida;

    // Barra roja de vida actual.
    sf::RectangleShape bar(sf::Vector2f(width * percent, 6.f));
    bar.setFillColor(sf::Color::Red);
    bar.setPosition(back.getPosition());

    window.draw(back);
    window.draw(bar);
}

void EnemyBase::applyDifficulty(float multiplier) {
    // Escala la vida por piso.
    maxVida = (int)(maxVida * multiplier);

    if (maxVida < 1) {
        maxVida = 1;
    }

    vida = maxVida;

    // La velocidad escala mas suave para que no se vuelva injusto.
    speed *= 1.f + ((multiplier - 1.f) * 0.45f);
}

bool EnemyBase::tryMove(sf::Vector2f movement, sf::RenderWindow& window) {
    // Mueve y luego corrige si quedo fuera de pantalla.
    hitbox.move(movement);
    clampToWindow(window);

    return true;
}

void EnemyBase::clampToWindow(sf::RenderWindow& window) {
    sf::FloatRect bounds = hitbox.getGlobalBounds();

    // Limite izquierdo.
    if (bounds.left < 0.f) {
        hitbox.move(-bounds.left, 0.f);
    }

    // Limite superior.
    if (bounds.top < 0.f) {
        hitbox.move(0.f, -bounds.top);
    }

    // Limite derecho.
    if (bounds.left + bounds.width > window.getSize().x) {
        hitbox.move(window.getSize().x - (bounds.left + bounds.width), 0.f);
    }

    // Limite inferior.
    if (bounds.top + bounds.height > window.getSize().y) {
        hitbox.move(0.f, window.getSize().y - (bounds.top + bounds.height));
    }

    // Como la hitbox pudo corregirse, sincronizamos sprite.
    syncSpritePosition();
}
