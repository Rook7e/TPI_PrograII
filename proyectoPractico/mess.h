#ifndef MESS_H_INCLUDED
#define MESS_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>

// Clase que representa la mugre/basura del juego.
// Se limpia con la aspiradora y entrega score/oro.
class mess {
private:
    // Hitbox base para detectar aspiradora.
    sf::CircleShape shape;

    // Sprite visual de la mugre.
    sf::Sprite sprite;

    // Texturas compartidas entre todas las mugres.
    static sf::Texture texture1;
    static sf::Texture texture2;
    static sf::Texture ghostTexture;

    // Flags para cargar texturas una sola vez.
    static bool texture1Loaded;
    static bool texture2Loaded;
    static bool ghostTextureLoaded;
    static bool triedLoad;

    bool hasSprite;
    int spriteType;

    // Densidad: cuanto llena la aspiradora.
    float density;

    // Recompensas al limpiar.
    int scoreValue;
    int goldValue;

    // Tiempo necesario para limpiar y progreso actual.
    float cleanTime;
    float cleanProgress;

public:
    // type -1 significa aleatorio.
    // type 0 y 1 son mugres normales.
    // type 2 es mugre que dejan enemigos.
    mess(sf::Vector2f position, int type = -1);

    // Datos usados por Progression.
    float getDensity();
    int getScoreValue();
    int getGoldValue();

    // Sistema de limpieza por tiempo.
    void updateCleaning(float deltaTime);
    bool isCleaned();
    float getCleanPercent();

    // Dibujo y colision.
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds();
};

#endif
