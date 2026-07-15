#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Player.h"
#include "circle.h"
#include "enemy.h"
#include "TileMap.h"
#include "Medkit.h"
#include "mess.h"
#include "AudioManager.h"
#include "MainMenu.h"
#include "Progression.h"
#include "UpgradeMenu.h"
#include "SaveSystem.h"
#include "PauseMenu.h"

// Informacion de cada sala del juego.
// Guarda que mapa carga, si ya fue visitada/limpiada y que puertas tiene.
struct RoomInfo {
    std::string groundFile;
    std::string assetsFile;

    bool visited;
    bool cleared;

    bool doorUp;
    bool doorDown;
    bool doorLeft;
    bool doorRight;
};

// Estados principales del juego.
// Sirven para separar menu, gameplay, pausa, mejoras, jefe y game over.
enum GameState {
    MainMenuState,
    BossIntroState,
    Playing,
    PausedState,
    UpgradeMenuState,
    GameOver
};

class Game {
private:
    // Ventana principal de SFML.
    sf::RenderWindow window;

    // Sistemas principales del juego.
    Player player;
    circle aspiradora;
    TileMap tileMap;
    AudioManager audio;
    MainMenu mainMenu;

    // Vectores de entidades activas en la sala actual.
    std::vector<EnemyChaser> chasers;
    std::vector<EnemyShooter> shooters;
    std::vector<EnemyThrower> throwers;
    std::vector<EnemyBoss> bosses;
    std::vector<mess> messes;
    std::vector<Medkit> medkits;

    // Clocks usados para controlar tiempos de spawn, dano y frames.
    sf::Clock spawnChaserClock;
    sf::Clock spawnShooterClock;
    sf::Clock spawnThrowerClock;
    sf::Clock vacuumDamageClock;
    sf::Clock medkitSpawnClock;
    sf::Clock frameClock;

    GameState gameState;

    // Datos de progreso viejo/mapa y control de limpieza.
    int currentMap;
    int enemiesKilled;
    int enemiesNeededForNextMap;
    bool waitingForCleaning;

    // Loop principal dividido en eventos, actualizacion y dibujo.
    void processEvents();
    void update(float deltaTime);
    void updatePlaying(float deltaTime);
    void updateGameOver();
    void draw();

    // Carga/reset de mapas.
    void loadMap(int mapNumber);
    void resetGame();
    void changeToMap2();

    // Posiciones aleatorias para spawns.
    sf::Vector2f randomSpawn();
    sf::Vector2f randomMessPosition();

    // Spawns iniciales de sala.
    void spawnInitialEnemies();
    void spawnInitialMess();

    // Bloques de actualizacion del gameplay.
    void updatePlayer(float deltaTime);
    void updateMessCleaning(float deltaTime);
    void updateEnemies(float deltaTime);
    void applyVacuumDamage();
    void removeDeadEnemies();
    void updateSpawns();
    void updateMedkits();
    void checkMapProgress();
    void checkMapChange();

    // Matriz de salas tipo Isaac.
    std::vector<std::vector<RoomInfo> > rooms;
    int currentRoomX;
    int currentRoomY;

    // Sistema de salas.
    void setupRooms();
    void enterRoom(int x, int y);
    void checkRoomCleared();
    void checkRoomTransition();

    // Sala del jefe y avance de piso.
    bool isBossRoom(int x, int y);
    bool areNormalRoomsCleared();
    void spawnBoss();

    sf::RectangleShape trapdoor;
    bool trapdoorActive;
    int currentFloor;

    void setupTrapdoor();
    void updateTrapdoor();
    void goToNextFloor();

    // Progresion y menu de mejoras.
    Progression progression;
    UpgradeMenu upgradeMenu;

    // Posicion segura para respawn.
    sf::Vector2f lastSafePlayerPosition;
    void setPlayerSafePosition(sf::Vector2f position);

    // Barra de basura/capacidad.
    void drawTrashBar();

    // Guardado y pausa.
    SaveSystem saveSystem;
    PauseMenu pauseMenu;
    GameState stateBeforePause;

    SaveData createSaveData();
    bool saveGame(int slot);
    bool loadGame(int slot);
    void startNewGame();
    int activeSaveSlot;
    void openSaveSlot(int slot);

    // Intro y HUD del jefe.
    sf::Clock bossIntroClock;
    sf::Font bossFont;
    bool bossFontLoaded;
    sf::Text bossNameText;

    void startBossIntro();
    void updateBossIntro();
    void drawBossIntroHud();
    void drawBossHealthBar();

    // Escalado de dificultad por piso.
    float getDifficultyMultiplier();

    // Funciones centralizadas para crear enemigos ya escalados.
    void addChaser(sf::Vector2f position);
    void addShooter(sf::Vector2f position);
    void addThrower(sf::Vector2f position);
    void addBoss(sf::Vector2f position);

public:
    Game();
    void run();
};

#endif
