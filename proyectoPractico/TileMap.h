#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Clase encargada de cargar, dibujar y manejar colisiones del mapa.
// Usa archivos CSV para representar tiles.
class TileMap {
private:
    // Tileset de pisos/paredes.
    sf::Texture buildingTileset;

    // Tileset de muebles/objetos.
    sf::Texture furnitureTileset;

    // Capas del mapa cargadas desde CSV.
    std::vector<std::vector<int> > groundLayer;
    std::vector<std::vector<int> > assetsLayer;

    // Tamano de cada tile en pixeles.
    int tileSize;

public:
    TileMap();

    // Carga las texturas de tiles.
    bool load();

    // Dibujo y colisiones.
    void draw(sf::RenderWindow& window);
    bool checkCollision(sf::FloatRect bounds);

    // Carga de capas desde archivos CSV.
    bool loadGroundLayer(const std::string& fileName);
    bool loadAssetsLayer(const std::string& fileName);
    bool loadLayer(const std::string& fileName, std::vector<std::vector<int>>& layer);

    // Dibuja ambas capas del mapa.
    void drawMap(sf::RenderWindow& window);

    // Manejo de muebles dentro del mapa.
    bool hasFurniture(int x, int y);
    void removeFurniture(int x, int y);
    int getFurnitureId(int x, int y);

    // Acceso a texturas para proyectiles de muebles.
    const sf::Texture& getBuildingTileset() const;
    const sf::Texture& getFurnitureTileset() const;

    // Busca muebles para que el thrower o boss puedan usarlos.
    bool findNearestFurniture(sf::Vector2f position, sf::Vector2i& result);

    // Linea de vision para saber si hay obstaculos entre dos puntos.
    bool hasLineOfSight(sf::Vector2f from, sf::Vector2f to);

    // Busca muebles en linea entre enemigo y jugador.
    bool findFurnitureInLine(
        sf::Vector2f throwerPos,
        sf::Vector2f playerPos,
        sf::Vector2i& result
    );

    // Indica si queda algun mueble en la sala.
    bool hasAnyFurniture();
};

#endif
