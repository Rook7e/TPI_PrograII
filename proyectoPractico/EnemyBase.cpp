#include "EnemyBase.h"
#include "VectorMath.h"

EnemyBase::EnemyBase(sf::Vector2f position, float radius, float newSpeed, int newMaxVida) {
    hitbox.setRadius(radius);
    hitbox.setOrigin(radius, radius);
    hitbox.setPosition(position);
    hitbox.setFillColor(sf::Color::Transparent);

    speed = newSpeed;
    maxVida = newMaxVida;
    vida = maxVida;
}

EnemyBase::~EnemyBase() {
}

void EnemyBase::syncSpritePosition() {
}

void EnemyBase::takeDamage(int damage, sf::Vector2f hitPosition) {
    if (isDead()) {
        return;
    }

    vida -= damage;

    sf::Vector2f knockDirection = normalize(hitbox.getPosition() - hitPosition);

    hitbox.move(knockDirection.x * 45.f, knockDirection.y * 45.f);
    syncSpritePosition();

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
    sf::RectangleShape back(sf::Vector2f(width, 6.f));
    back.setFillColor(sf::Color(70, 20, 20));
    back.setPosition(
        hitbox.getPosition().x - width / 2.f,
        hitbox.getPosition().y - hitbox.getRadius() - 14.f
    );

    float percent = (float)vida / maxVida;

    sf::RectangleShape bar(sf::Vector2f(width * percent, 6.f));
    bar.setFillColor(sf::Color::Red);
    bar.setPosition(back.getPosition());

    window.draw(back);
    window.draw(bar);
}
