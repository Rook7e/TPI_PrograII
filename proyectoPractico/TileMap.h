#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap {
private:
    sf::Texture tileset;
    std::vector<std::vector<int> > groundLayer;
    std::vector<std::vector<int> > assetsLayer;
    int tileSize;

public:
    TileMap();

    bool load();
    void draw(sf::RenderWindow& window);
    bool checkCollision(sf::FloatRect bounds);
    bool loadGroundLayer(const std::string& fileName);
    bool loadAssetsLayer(const std::string& fileName);
    void drawMap(sf::RenderWindow& window);
    bool loadLayer(const std::string& fileName, std::vector<std::vector<int>>& layer);

    bool hasFurniture(int x, int y);
    void removeFurniture(int x, int y);

    const sf::Texture& getTileset() const;

    bool findNearestFurniture(
    sf::Vector2f position,
    sf::Vector2i& result
    );

    int getFurnitureId(int x, int y);

    bool hasLineOfSight(sf::Vector2f from, sf::Vector2f to);

    bool findFurnitureInLine(
    sf::Vector2f throwerPos,
    sf::Vector2f playerPos,
    sf::Vector2i& result
    );

};

#endif
