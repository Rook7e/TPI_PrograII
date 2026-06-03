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

    tileMap.loadGroundLayer("maps/map2-groundLayer.csv");
    tileMap.loadAssetsLayer("maps/map2-assetsLayer.csv");

    srand((unsigned)time(NULL));

    GameState gameState = Playing;

    std::vector<EnemyChaser> chasers;
    std::vector<EnemyShooter> shooters;
    std::vector<mess> messes;
    std::vector<Medkit> medkits;

    chasers.push_back(EnemyChaser(sf::Vector2f(700.f, 100.f)));
    shooters.push_back(EnemyShooter(sf::Vector2f(650.f, 500.f)));

    for (int i = 0; i < 12; i++) {
    messes.push_back(mess(randomMessPosition()));
}

    sf::Clock spawnChaserClock;
    sf::Clock spawnShooterClock;
    sf::Clock vacuumDamageClock;
    sf::Clock medkitSpawnClock;
    sf::Clock clock;

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

                if (hitSomething) {
                    vacuumDamageClock.restart();
                }
            }

            for (int i = 0; i < chasers.size(); i++) {
            if (chasers[i].isDead()) {
                messes.push_back(mess(chasers[i].getPosition(), 2));
                chasers.erase(chasers.begin() + i);
                i--;
    }
}

            for (int i = 0; i < shooters.size(); i++) {
            if (shooters[i].isDead()) {
                messes.push_back(mess(shooters[i].getPosition(), 2));
                shooters.erase(shooters.begin() + i);
                i--;
    }
}

            if (spawnChaserClock.getElapsedTime().asSeconds() >= 3.f && chasers.size() < 5) {
                chasers.push_back(EnemyChaser(randomSpawn(window)));
                spawnChaserClock.restart();
            }

            if (spawnShooterClock.getElapsedTime().asSeconds() >= 6.f && shooters.size() < 3) {
                shooters.push_back(EnemyShooter(randomSpawn(window)));
                spawnShooterClock.restart();
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

                chasers.clear();
                shooters.clear();
                medkits.clear();
                messes.clear();

                for (int i = 0; i < 12; i++) {
                    messes.push_back(mess(randomMessPosition()));
                }

                chasers.push_back(EnemyChaser(sf::Vector2f(700.f, 100.f)));
                shooters.push_back(EnemyShooter(sf::Vector2f(650.f, 500.f)));

                spawnChaserClock.restart();
                spawnShooterClock.restart();
                vacuumDamageClock.restart();
                medkitSpawnClock.restart();

                window.setTitle("Proyecto Practico");
                gameState = Playing;
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

        player.drawStamina(window);
        player.drawLife(window);

        window.display();
    }

    return 0;
}
