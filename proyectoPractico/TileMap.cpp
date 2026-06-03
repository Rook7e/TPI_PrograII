#include <fstream>
#include <iostream>
#include <vector>
#include "TileMap.h"


using namespace std;

TileMap::TileMap() {



}

bool TileMap::load() {
    return tileset.loadFromFile("assets/32x32/A2_32x32.png");
}

bool TileMap::loadGroundLayer(const std::string& fileName) {
    return loadLayer(fileName, groundLayer);
}

bool TileMap::loadAssetsLayer(const std::string& fileName) {
    return loadLayer(fileName, assetsLayer);
}

bool TileMap::checkCollision(sf::FloatRect bounds) {
    for (int y = 0; y < assetsLayer.size(); y++) {
        for (int x = 0; x < assetsLayer[y].size(); x++) {
            if (assetsLayer[y][x] == 6 || assetsLayer[y][x] == 12) {
                sf::FloatRect wall(
                    (float)(x * tileSize),
                    (float)(y * tileSize),
                    (float)tileSize,
                    (float)tileSize
                );

                if (bounds.intersects(wall)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool TileMap::loadLayer(const std::string& fileName, std::vector<std::vector<int>>& layer)
{
    std::fstream file(fileName);

    if (!file.is_open())
    {
        cout << "No se pudo abrir " << fileName << endl;
        return false;
    }

    layer.clear();

    string line;

    while (getline(file, line))
    {
        vector<int> row;
        string number;

        for (char c : line)
        {
            if (c == ',')
            {
                row.push_back(stoi(number));
                number.clear();
            }
            else
            {
                number += c;
            }
        }

        if (!number.empty())
        {
            row.push_back(stoi(number));
        }
        layer.push_back(row);
    }
    return true;
}

void TileMap::drawMap(sf::RenderWindow& window)
{
    tileSize = 32;

    const int tileVoid = -1;
    const int tileFloor = 0;
    const int tileWall = 6;
    const int tileCarpet = 49;
    const int tileLeftUpCarpet = 64;
    const int tileRightUpCarpet = 65;
    const int tileLeftDownCarpet = 80;
    const int tileRightDownCarpet = 81;
    const int tileForniture = 12;


    sf::Sprite tile;
    tile.setTexture(tileset);

    for (size_t y = 0; y < groundLayer.size(); y++)
    {
        for (size_t x = 0; x < groundLayer[y].size(); x++)
        {
            int value = groundLayer[y][x];


            // Piso
            if (value == tileFloor)
            {
                tile.setTextureRect(sf::IntRect(0, 0, 32, 32));
            }
            // Pared
            else if (value == tileWall)
            {
                tile.setTextureRect(sf::IntRect(192, 0, 32, 32));
            }
            else if (value == tileCarpet)
            {
                tile.setTextureRect(sf::IntRect(32, 96, 32, 32));
            }
            else if (value == tileLeftUpCarpet)
            {
                tile.setTextureRect(sf::IntRect(0, 128, 32, 32));
            }
            else if (value == tileRightUpCarpet)
            {
                tile.setTextureRect(sf::IntRect(32, 128, 32, 32));
            }
            else if (value == tileLeftDownCarpet)
            {
                tile.setTextureRect(sf::IntRect(0, 160, 32, 32));
            }
            else if (value == tileRightDownCarpet)
            {
                tile.setTextureRect(sf::IntRect(32, 160, 32, 32));
            }


            tile.setPosition((float)(x * tileSize), (float)(y * tileSize));
            window.draw(tile);
        }
    }

    for (size_t y = 0; y < assetsLayer.size(); y++)
    {
        for (size_t x = 0; x < assetsLayer[y].size(); x++)
        {
            int value = assetsLayer[y][x];

            if (value == tileForniture)
            {
                tile.setTextureRect(sf::IntRect(384, 0, 32, 32));
            }
            else continue;

            tile.setPosition(x * tileSize, y * tileSize);
            window.draw(tile);
        }
    }
}
