#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap {
private:
    sf::Texture tileset;
    std::vector<std::vector<int> > map;
    int tileSize;

public:
    TileMap();

    bool load();
    void draw(sf::RenderWindow& window);
    bool checkCollision(sf::FloatRect bounds);
    bool loadMapFile(const std::string& fileName);
    void drawMap(sf::RenderWindow& window);
};

#endif
