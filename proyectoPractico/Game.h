#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.h"
#include "circle.h"
#include "enemy.h"
#include "TileMap.h"
#include "Medkit.h"
#include "mess.h"

enum GameState {
    Playing,
    GameOver
};

class Game {
private:
    sf::RenderWindow window;

    Player player;
    circle aspiradora;
    TileMap tileMap;

    std::vector<EnemyChaser> chasers;
    std::vector<EnemyShooter> shooters;
    std::vector<EnemyThrower> throwers;
    std::vector<mess> messes;
    std::vector<Medkit> medkits;

    sf::Clock spawnChaserClock;
    sf::Clock spawnShooterClock;
    sf::Clock spawnThrowerClock;
    sf::Clock vacuumDamageClock;
    sf::Clock medkitSpawnClock;
    sf::Clock frameClock;

    GameState gameState;

    int currentMap;
    int enemiesKilled;
    int enemiesNeededForNextMap;
    bool waitingForCleaning;

    void processEvents();
    void update(float deltaTime);
    void updatePlaying(float deltaTime);
    void updateGameOver();
    void draw();

    void loadMap(int mapNumber);
    void resetGame();
    void changeToMap2();

    sf::Vector2f randomSpawn();
    sf::Vector2f randomMessPosition();

    void spawnInitialEnemies();
    void spawnInitialMess();

    void updatePlayer(float deltaTime);
    void updateMessCleaning();
    void updateEnemies(float deltaTime);
    void applyVacuumDamage();
    void removeDeadEnemies();
    void updateSpawns();
    void updateMedkits();
    void checkMapProgress();
    void checkMapChange();

public:
    Game();
    void run();
};

#endif
