#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Player.h"
#include "circle.h"
#include "enemy.h"
#include "TileMap.h"
#include "Medkit.h"
#include "mess.h"

using namespace std;

void loadMap(TileMap& tileMap, int mapNumber) {
    if (mapNumber == 1) {
        tileMap.loadGroundLayer("maps/map1-groundLayer.csv");
        tileMap.loadAssetsLayer("maps/map1-assetsLayer.csv");
    } else if (mapNumber == 2) {
        tileMap.loadGroundLayer("maps/map2-groundLayer.csv");
        tileMap.loadAssetsLayer("maps/map2-assetsLayer.csv");
    }
}

enum GameState {
    Playing,
    GameOver
};

sf::Vector2f randomSpawn(sf::RenderWindow& window) {
    int side = rand() % 4;
    float w = (float)window.getSize().x;
    float h = (float)window.getSize().y;

    if (side == 0) return sf::Vector2f((float)(rand() % (int)w), -30.f);
    if (side == 1) return sf::Vector2f(w + 30.f, (float)(rand() % (int)h));
    if (side == 2) return sf::Vector2f((float)(rand() % (int)w), h + 30.f);

    return sf::Vector2f(-30.f, (float)(rand() % (int)h));
}

sf::Vector2f randomMessPosition() {
    float x = 60.f + (rand() % 680);
    float y = 60.f + (rand() % 480);

    return sf::Vector2f(x, y);
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Proyecto Practico");
    window.setFramerateLimit(60);

    Player player;
    circle aspiradora;


    TileMap tileMap;

    if (!tileMap.load()) {
    return -1;
    }

    loadMap(tileMap, 1);

    srand((unsigned)time(NULL));

    GameState gameState = Playing;

    std::vector<EnemyChaser> chasers;
    std::vector<EnemyShooter> shooters;
    std::vector<EnemyThrower> throwers;
    std::vector<mess> messes;
    std::vector<Medkit> medkits;

    chasers.push_back(EnemyChaser(sf::Vector2f(700.f, 100.f)));
    shooters.push_back(EnemyShooter(sf::Vector2f(650.f, 500.f)));
    throwers.push_back(EnemyThrower(sf::Vector2f(200.f, 500.f)));

    for (int i = 0; i < 12; i++) {
    messes.push_back(mess(randomMessPosition()));
}

    sf::Clock spawnChaserClock;
    sf::Clock spawnShooterClock;
    sf::Clock spawnThrowerClock;
    sf::Clock vacuumDamageClock;
    sf::Clock medkitSpawnClock;
    sf::Clock clock;

    int currentMap = 1;
    int enemiesKilled = 0;
    int enemiesNeededForNextMap = 6;
    bool waitingForCleaning = false;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        switch (gameState) {
        case Playing: {
            sf::Vector2f oldPlayerPosition = player.getPosition();

            player.movement();

            if (tileMap.checkCollision(player.getBounds())) {
                player.setPosition(oldPlayerPosition);
            }

            player.update(deltaTime);

            aspiradora.update(window, player.getCenter());

            messes.erase(
            std::remove_if(
                messes.begin(),
                messes.end(),
                [&aspiradora](mess& dirt) {
                    return dirt.getBounds().intersects(aspiradora.getBounds());
                }
            ),
            messes.end()
        );

            for (int i = 0; i < chasers.size(); i++) {
                chasers[i].update(deltaTime, player);
            }

            for (int i = 0; i < shooters.size(); i++) {
                shooters[i].update(deltaTime, player, aspiradora, window, tileMap);
            }

            for (int i = 0; i < throwers.size(); i++) {
                throwers[i].update(deltaTime, player, aspiradora, window, tileMap);
            }

            if (vacuumDamageClock.getElapsedTime().asSeconds() >= 0.25f) {
                bool hitSomething = false;

                for (int i = 0; i < chasers.size(); i++) {
                    if (!chasers[i].isDead() &&
                        aspiradora.getBounds().intersects(chasers[i].getBounds())) {
                        chasers[i].takeDamage(1, aspiradora.getPosition());
                        hitSomething = true;
                    }
                }

                for (int i = 0; i < shooters.size(); i++) {
                    if (!shooters[i].isDead() &&
                        aspiradora.getBounds().intersects(shooters[i].getBounds())) {
                        shooters[i].takeDamage(1, aspiradora.getPosition());
                        hitSomething = true;
                    }
                }
                for (int i = 0; i < throwers.size(); i++) {
                    if (!throwers[i].isDead() &&
                        aspiradora.getBounds().intersects(throwers[i].getBounds())) {
                        throwers[i].takeDamage(1, aspiradora.getPosition());
                        hitSomething = true;
                    }
                }

                if (hitSomething) {
                    vacuumDamageClock.restart();
                }
            }

            for (int i = 0; i < chasers.size(); i++) {
                if (chasers[i].isDead()) {
                    messes.push_back(mess(chasers[i].getPosition(), 2));
                    chasers.erase(chasers.begin() + i);
                    enemiesKilled++;
                    i--;
                }
            }

            for (int i = 0; i < shooters.size(); i++) {
                if (shooters[i].isDead()) {
                    messes.push_back(mess(shooters[i].getPosition(), 2));
                    shooters.erase(shooters.begin() + i);
                    enemiesKilled++;
                    i--;
                }
            }

            if (enemiesKilled >= enemiesNeededForNextMap) {
                waitingForCleaning = true;
            }

            throwers.erase(
                std::remove_if(
                    throwers.begin(),
                    throwers.end(),
                    [](EnemyThrower& enemy) {
                        return enemy.isDead();
                    }
                ),
                throwers.end()
            );

            if (spawnChaserClock.getElapsedTime().asSeconds() >= 3.f && chasers.size() < 5) {
                chasers.push_back(EnemyChaser(randomSpawn(window)));
                spawnChaserClock.restart();
            }

            if (!waitingForCleaning &&
                spawnShooterClock.getElapsedTime().asSeconds() >= 6.f &&
                shooters.size() < 3) {
                shooters.push_back(EnemyShooter(randomSpawn(window)));
                spawnShooterClock.restart();
                        }


            if (waitingForCleaning &&
                messes.empty() &&
                chasers.empty() &&
                shooters.empty() &&
                currentMap == 1) {
                currentMap = 2;
                loadMap(tileMap, 2);

                enemiesKilled = 0;
                waitingForCleaning = false;

                chasers.clear();
                shooters.clear();
                medkits.clear();
                messes.clear();

                player.setPosition(sf::Vector2f(100.f, 100.f));

                chasers.push_back(EnemyChaser(sf::Vector2f(700.f, 100.f)));
                shooters.push_back(EnemyShooter(sf::Vector2f(650.f, 500.f)));

                for (int i = 0; i < 12; i++) {
                    messes.push_back(mess(randomMessPosition()));
                }

                spawnChaserClock.restart();
                spawnShooterClock.restart();
                medkitSpawnClock.restart();
                vacuumDamageClock.restart();

                window.setTitle("Mapa 2");
            }

            if (spawnThrowerClock.getElapsedTime().asSeconds() >= 8.f && throwers.size() < 2) {
                throwers.push_back(EnemyThrower(randomSpawn(window)));
                spawnThrowerClock.restart();
            }

            if (medkitSpawnClock.getElapsedTime().asSeconds() >= 5.f && medkits.size() < 3) {
                float x = 60.f + (rand() % 680);
                float y = 60.f + (rand() % 480);

                medkits.push_back(Medkit(sf::Vector2f(x, y)));
                medkitSpawnClock.restart();
            }

            medkits.erase(
                std::remove_if(
                    medkits.begin(),
                    medkits.end(),
                    [&player](Medkit& medkit) {
                        if (medkit.getBounds().intersects(player.getBounds())) {
                            player.heal(1);
                            return true;
                        }

                        return false;
                    }
                ),
                medkits.end()
            );

            if (player.isDead()) {
                gameState = GameOver;
                window.setTitle("Estas muerto - Presiona R para reiniciar");
            }

            break;
        }

        case GameOver: {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                player.reset();

                tileMap.loadGroundLayer("maps/map1-groundLayer.csv");
                tileMap.loadAssetsLayer("maps/map1-assetsLayer.csv");

                chasers.clear();
                shooters.clear();
                throwers.clear();
                medkits.clear();
                messes.clear();

                for (int i = 0; i < 12; i++) {
                    messes.push_back(mess(randomMessPosition()));
                }

                chasers.push_back(EnemyChaser(sf::Vector2f(700.f, 100.f)));
                shooters.push_back(EnemyShooter(sf::Vector2f(650.f, 500.f)));
                throwers.push_back(EnemyThrower(sf::Vector2f(200.f, 500.f)));

                spawnChaserClock.restart();
                spawnShooterClock.restart();
                spawnThrowerClock.restart();
                vacuumDamageClock.restart();
                medkitSpawnClock.restart();

                window.setTitle("Proyecto Practico");
                gameState = Playing;

                currentMap = 1;
                enemiesKilled = 0;
                waitingForCleaning = false;
                loadMap(tileMap, 1);
                            }

            break;
        }
        }

        window.clear(sf::Color::Black);

        tileMap.drawMap(window);

        for (int i = 0; i < messes.size(); i++) {
            messes[i].draw(window);
}

        for (int i = 0; i < medkits.size(); i++) {
            medkits[i].draw(window);
        }

        player.draw(window);
        aspiradora.draw(window);

        for (int i = 0; i < chasers.size(); i++) {
            chasers[i].draw(window);
        }

        for (int i = 0; i < shooters.size(); i++) {
            shooters[i].draw(window);
        }

        for (int i = 0; i < throwers.size(); i++) {
            throwers[i].draw(window);
        }

        player.drawStamina(window);
        player.drawLife(window);

        window.display();
    }

    return 0;
}
