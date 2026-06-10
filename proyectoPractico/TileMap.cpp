#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "TileMap.h"



using namespace std;

TileMap::TileMap() {

    tileSize = 32;

}

const sf::Texture& TileMap::getTileset() const
{
    return tileset;
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
            if (assetsLayer[y][x] == 6 || assetsLayer[y][x] == 12 || assetsLayer[y][x] == 14|| assetsLayer[y][x] == 4 ) {
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
    const int tileBlackWall = 6;
    const int tileRedWall = 4;
    const int tileRedCarpet = 49;
    const int tileLeftUpRedCarpet = 64;
    const int tileRightUpRedCarpet = 65;
    const int tileLeftDownRedCarpet = 80;
    const int tileRightDownRedCarpet = 81;
    const int tileBrownCarpet = 51;
    const int tileLeftUpBrownCarpet = 66;
    const int tileRightUpBrownCarpet = 67;
    const int tileLeftDownBrownCarpet = 82;
    const int tileRightDownBrownCarpet = 83;
    const int tileWoodForniture = 12;
    const int tileBlackForniture = 14;


    sf::Sprite tile;
    tile.setTexture(tileset);

    for (size_t y = 0; y < groundLayer.size(); y++)
    {
        for (size_t x = 0; x < groundLayer[y].size(); x++)
        {
            int value = groundLayer[y][x];



            if (value == tileFloor) // Piso
            {
                tile.setTextureRect(sf::IntRect(0, 0, 32, 32));
            }
            else if (value == tileBlackWall) // Black wall
            {
                tile.setTextureRect(sf::IntRect(192, 0, 32, 32));
            }
            else if (value == tileRedWall) // Red wall
            {
                tile.setTextureRect(sf::IntRect(128, 0, 32, 32));
            }
            else if (value == tileRedCarpet) // Red carpet
            {
                tile.setTextureRect(sf::IntRect(32, 96, 32, 32));
            }
            else if (value == tileLeftUpRedCarpet)
            {
                tile.setTextureRect(sf::IntRect(0, 128, 32, 32));
            }
            else if (value == tileRightUpRedCarpet)
            {
                tile.setTextureRect(sf::IntRect(32, 128, 32, 32));
            }
            else if (value == tileLeftDownRedCarpet)
            {
                tile.setTextureRect(sf::IntRect(0, 160, 32, 32));
            }
            else if (value == tileRightDownRedCarpet)
            {
                tile.setTextureRect(sf::IntRect(32, 160, 32, 32));
            }
            else if (value == tileBrownCarpet) // Brown carpet
            {
                tile.setTextureRect(sf::IntRect(96, 96, 32, 32));
            }
            else if (value == tileLeftUpBrownCarpet)
            {
                tile.setTextureRect(sf::IntRect(64, 128, 32, 32));
            }
            else if (value == tileRightUpBrownCarpet)
            {
                tile.setTextureRect(sf::IntRect(96, 128, 32, 32));
            }
            else if (value == tileLeftDownBrownCarpet)
            {
                tile.setTextureRect(sf::IntRect(64, 160, 32, 32));
            }
            else if (value == tileRightDownBrownCarpet)
            {
                tile.setTextureRect(sf::IntRect(96, 160, 32, 32));
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

            if (value == tileWoodForniture)
            {
                tile.setTextureRect(sf::IntRect(384, 0, 32, 32));
            }

            else if (value == tileBlackForniture)
            {
                tile.setTextureRect(sf::IntRect(448, 0, 32, 32));
            }
            else continue;

            tile.setPosition(x * tileSize, y * tileSize);
            window.draw(tile);
        }
    }
}

int TileMap::getFurnitureId(int x, int y)
{
    if(y < 0 || y >= assetsLayer.size())
        return -1;

    if(x < 0 || x >= assetsLayer[y].size())
        return -1;

    return assetsLayer[y][x];
}

bool TileMap::hasFurniture(int x, int y)
{
    if(y < 0 || y >= assetsLayer.size())
        return false;

    if(x < 0 || x >= assetsLayer[y].size())
        return false;

    return assetsLayer[y][x] == 12 || assetsLayer[y][x] == 14;
}

void TileMap::removeFurniture(int x, int y)
{
    if(y < 0 || y >= assetsLayer.size())
        return;

    if(x < 0 || x >= assetsLayer[y].size())
        return;

    assetsLayer[y][x] = -1;
}

bool TileMap::findNearestFurniture(
    sf::Vector2f position,
    sf::Vector2i& result)
{
    float bestDistance = 999999.f;
    bool found = false;

    for(int y = 0; y < assetsLayer.size(); y++)
    {
        for(int x = 0; x < assetsLayer[y].size(); x++)
        {
            if(assetsLayer[y][x] == 12 || assetsLayer[y][x] == 14)
            {
                sf::Vector2f furniturePos(
                    x * tileSize + tileSize / 2.f,
                    y * tileSize + tileSize / 2.f
                );

                float dx = furniturePos.x - position.x;
                float dy = furniturePos.y - position.y;

                float distance = std::sqrt(dx * dx + dy * dy);

                if(distance < bestDistance)
                {
                    bestDistance = distance;
                    result.x = x;
                    result.y = y;
                    found = true;
                }
            }
        }
    }

    return found;
}

bool TileMap::hasLineOfSight(sf::Vector2f from, sf::Vector2f to) {
    sf::Vector2f direction = to - from;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0.f) {
        return true;
    }

    direction.x /= distance;
    direction.y /= distance;

    float step = 8.f;

    for (float traveled = 0.f; traveled < distance; traveled += step) {
        sf::Vector2f point = from + direction * traveled;

        sf::FloatRect rayPoint(point.x - 2.f, point.y - 2.f, 4.f, 4.f);

        if (checkCollision(rayPoint)) {
            return false;
        }
    }

    return true;
}

bool TileMap::findFurnitureInLine(sf::Vector2f throwerPos, sf::Vector2f playerPos, sf::Vector2i& result) {
    sf::Vector2f direction = playerPos - throwerPos;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0.f) {
        return true;
    }

    direction.x /= distance;
    direction.y /= distance;

    float step = 8.f;

    for (float traveled = 0.f; traveled < distance; traveled += step) {
        sf::Vector2f point = throwerPos + direction * traveled;

        int tileX = point.x / tileSize;
        int tileY = point.y / tileSize;

        if(hasFurniture(tileX, tileY))
        {
            result.x = tileX;
            result.y = tileY;
            return true;
        }
    }

    return false;
}

bool TileMap::hasAnyFurniture()
{
    for (int y = 0; y < assetsLayer.size(); y++)
    {
        for (int x = 0; x < assetsLayer[y].size(); x++)
        {
            if (assetsLayer[y][x] == 12 ||
                assetsLayer[y][x] == 14)
            {
                return true;
            }
        }
    }

    return false;
}
