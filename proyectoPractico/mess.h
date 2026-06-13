#ifndef MESS_H_INCLUDED
#define MESS_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

class mess {
private:
    sf::CircleShape shape;
    sf::Sprite sprite;

    static sf::Texture texture1;
    static sf::Texture texture2;
    static sf::Texture ghostTexture;

    static bool texture1Loaded;
    static bool texture2Loaded;
    static bool ghostTextureLoaded;
    static bool triedLoad;

    bool hasSprite;
    int spriteType;

    float density;
    int scoreValue;
    int goldValue;

    float cleanTime;
    float cleanProgress;

public:
    mess(sf::Vector2f position, int type = -1);

    float getDensity();
    int getScoreValue();
    int getGoldValue();

    void updateCleaning(float deltaTime);
    bool isCleaned();
    float getCleanPercent();

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
};

#endif
