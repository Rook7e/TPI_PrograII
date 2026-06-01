#include <fstream>
#include <iostream>
#include <vector>
#include "TileMap.h"


using namespace std;

TileMap::TileMap() {

    map = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,2},
        {2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2},
        {2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2},
        {2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,2},
        {2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2},
        {2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2},
        {2,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
    };

}

bool TileMap::load() {
    return tileset.loadFromFile("assets/32x32/A2_32x32.png");
}

bool TileMap::checkCollision(sf::FloatRect bounds) {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == 2) {
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


bool TileMap::loadMapFile(const std::string& fileName)
{
    fstream file(fileName);
    if (!file.is_open())
    {
        cout << "No se pudo abrir el archivo" << endl;
        return false;
    }
    map.clear();
    string line;

    while (getline(file, line))
    {
        vector<int> row;
        string number = "";

        for (char c : line)
        {
            if (c == ' ')
            {
                if (!number.empty())
                {
                    row.push_back(stoi(number));
                    number = "";
                }
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
        map.push_back(row);
    }

    file.close();
    return true;
}

void TileMap::drawMap(sf::RenderWindow& window)
{
    tileSize = 32;

    sf::Sprite tile;
    tile.setTexture(tileset);

    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map[y].size(); x++)
        {
            int value = map[y][x];


            // Piso
            if (value == 1)
            {
                tile.setTextureRect(sf::IntRect(0, 0, 32, 32));
            }
            // Pared
            else if (value == 2)
            {
                tile.setTextureRect(sf::IntRect(192, 0, 32, 32));
            }

            tile.setPosition((float)(x * tileSize), (float)(y * tileSize));
            window.draw(tile);
        }
    }
}
