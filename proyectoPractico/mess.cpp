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
}

sf::FloatRect mess::getBounds() {
    return shape.getGlobalBounds();
}
