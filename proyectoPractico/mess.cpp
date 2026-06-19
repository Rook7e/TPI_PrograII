#include "mess.h"
#include <cstdlib>
#include <iostream>

sf::Texture mess::texture1;
sf::Texture mess::texture2;
sf::Texture mess::ghostTexture;

bool mess::texture1Loaded = false;
bool mess::texture2Loaded = false;
bool mess::ghostTextureLoaded = false;
bool mess::triedLoad = false;

mess::mess(sf::Vector2f position, int type) {
    shape.setRadius(14.f);
    shape.setFillColor(sf::Color(70, 45, 25));
    shape.setOrigin(14.f, 14.f);
    shape.setPosition(position);

    hasSprite = false;

    if (!triedLoad) {
        texture1Loaded = texture1.loadFromFile("assets/Extras/mess.png");
        texture2Loaded = texture2.loadFromFile("assets/Extras/mess2.png");
        ghostTextureLoaded = ghostTexture.loadFromFile("assets/Extras/cum.png");

        if (!texture1Loaded) {
            std::cout << "Error al cargar assets/mess.png" << std::endl;
        }

        if (!texture2Loaded) {
            std::cout << "Error al cargar assets/mess2.png" << std::endl;
        }

        if (!ghostTextureLoaded) {
            std::cout << "Error al cargar assets/cum.png" << std::endl;
        }

        triedLoad = true;
    }

    if (type == -1) {
        spriteType = rand() % 2;
    } else {
        spriteType = type;
    }

        if (spriteType == 0) {
            density = 1.f;
            scoreValue = 10;
            goldValue = 1;
            cleanTime = 0.25f;
        } else if (spriteType == 1) {
            density = 2.f;
            scoreValue = 18;
            goldValue = 2;
            cleanTime = 0.7f;
        } else {
            density = 3.f;
            scoreValue = 30;
            goldValue = 4;
            cleanTime = 1.2f;
        }

cleanProgress = 0.f;

    sf::Texture* selectedTexture = NULL;

    if (spriteType == 0 && texture1Loaded) {
        selectedTexture = &texture1;
    } else if (spriteType == 1 && texture2Loaded) {
        selectedTexture = &texture2;
    } else if (spriteType == 2 && ghostTextureLoaded) {
        selectedTexture = &ghostTexture;
    }

    if (selectedTexture != NULL) {
        sprite.setTexture(*selectedTexture);

        sf::Vector2u textureSize = selectedTexture->getSize();

        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

        float scale = 32.f / textureSize.x;
        sprite.setScale(scale, scale);
        sprite.setPosition(position);

        shape.setFillColor(sf::Color::Transparent);
        hasSprite = true;
    }
}

void mess::draw(sf::RenderWindow& window) {
    if (hasSprite) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }

    if (cleanProgress > 0.f && cleanProgress < cleanTime) {
        sf::RectangleShape back(sf::Vector2f(28.f, 4.f));
        back.setFillColor(sf::Color(35, 25, 20));
        back.setPosition(shape.getPosition().x - 14.f, shape.getPosition().y - 22.f);

        sf::RectangleShape bar(sf::Vector2f(28.f * getCleanPercent(), 4.f));
        bar.setFillColor(sf::Color(180, 140, 80));
        bar.setPosition(back.getPosition());

        window.draw(back);
        window.draw(bar);
    }
}

sf::FloatRect mess::getBounds() {
    return shape.getGlobalBounds();
}

float mess::getDensity() {
    return density;
}

int mess::getScoreValue() {
    return scoreValue;
}

int mess::getGoldValue() {
    return goldValue;
}

void mess::updateCleaning(float deltaTime) {
    cleanProgress += deltaTime;

    if (cleanProgress > cleanTime) {
        cleanProgress = cleanTime;
    }
}

bool mess::isCleaned() {
    return cleanProgress >= cleanTime;
}

float mess::getCleanPercent() {
    if (cleanTime <= 0.f) {
        return 1.f;
    }

    return cleanProgress / cleanTime;
}
