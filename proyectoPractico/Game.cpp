#include "Game.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

Game::Game(): window(sf::VideoMode(1152, 864), "Proyecto Practico")
{
    window.setFramerateLimit(60);
    std::srand((unsigned)std::time(NULL));
    audio.load();
    audio.playMenuMusic();
    if (!tileMap.load()) {
        window.close();
    }

    gameState = MainMenuState;
    enemiesKilled = 0;
    enemiesNeededForNextMap = 6;
    waitingForCleaning = false;

    currentFloor = 1;
    trapdoorActive = false;
    setupTrapdoor();

    currentRoomX = 1;
    currentRoomY = 1;

    setupRooms();
    enterRoom(currentRoomX, currentRoomY);
    setPlayerSafePosition(sf::Vector2f(576.f, 432.f));

    currentFloor = 1;
    trapdoorActive = false;
    setupTrapdoor();
    aspiradora.setMaxDistance(progression.getVacuumRange());
    activeSaveSlot = 0;

    bossFontLoaded = bossFont.loadFromFile("assets/Fonts/arial.ttf");

    if (bossFontLoaded) {
        bossNameText.setFont(bossFont);
        bossNameText.setString("EL JEFE");
        bossNameText.setCharacterSize(28);
        bossNameText.setFillColor(sf::Color(230, 220, 210));
    }
}


SaveData Game::createSaveData() {
    SaveData data = {};

    data.score = progression.getScore();
    data.gold = progression.getGold();
    data.deaths = progression.getDeaths();
    data.currentFloor = currentFloor;
    data.vacuumDamageLevel = progression.getVacuumDamageLevel();
    data.vacuumRangeLevel = progression.getVacuumRangeLevel();
    data.vacuumCapacityLevel = progression.getVacuumCapacityLevel();
    data.trashStored = progression.getTrashStored();

    data.currentRoomX = currentRoomX;
    data.currentRoomY = currentRoomY;
    data.playerX = player.getPosition().x;
    data.playerY = player.getPosition().y;

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            data.roomsCleared[y * 3 + x] = rooms[y][x].cleared;
        }
    }

    data.chaserCount = (int)chasers.size();
    data.shooterCount = (int)shooters.size();
    data.throwerCount = (int)throwers.size();
    data.bossCount = (int)bosses.size();

    return data;
}

bool Game::saveGame(int slot) {
    return saveSystem.save(slot, createSaveData());
}

bool Game::loadGame(int slot) {
    SaveData data = {};
    if (!saveSystem.load(slot, data)) {
        return false;
    }

    progression.loadState(
        data.score,
        data.gold,
        data.deaths,
        data.currentFloor,
        data.vacuumDamageLevel,
        data.vacuumRangeLevel,
        data.vacuumCapacityLevel,
        data.trashStored
    );

    currentFloor = data.currentFloor;
    setupRooms();

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            rooms[y][x].cleared = data.roomsCleared[y * 3 + x];
        }
    }

    int roomX = data.currentRoomX;
    int roomY = data.currentRoomY;

    if (roomX < 0 || roomX > 2) roomX = 1;
    if (roomY < 0 || roomY > 2) roomY = 1;

    enterRoom(roomX, roomY);
    chasers.clear();
    shooters.clear();
    throwers.clear();
    bosses.clear();

    for (int i = 0; i < data.chaserCount; i++) {
        chasers.push_back(
            EnemyChaser(sf::Vector2f(650.f + i * 35.f, 250.f))
        );
    }

    for (int i = 0; i < data.shooterCount; i++) {
        shooters.push_back(
            EnemyShooter(sf::Vector2f(700.f + i * 35.f, 500.f))
        );
    }

    for (int i = 0; i < data.throwerCount; i++) {
        throwers.push_back(
            EnemyThrower(sf::Vector2f(250.f + i * 35.f, 500.f))
        );
    }

    for (int i = 0; i < data.bossCount; i++) {
        bosses.push_back(
            EnemyBoss(sf::Vector2f(576.f, 360.f))
        );
}
    setPlayerSafePosition(sf::Vector2f(data.playerX, data.playerY));
    aspiradora.setMaxDistance(progression.getVacuumRange());

    trapdoorActive = false;
    gameState = Playing;
    frameClock.restart();
    return true;
}

    void Game::openSaveSlot(int slot) {
    activeSaveSlot = slot;
    pauseMenu.setSelectedSlot(slot);

    if (saveSystem.exists(slot)) {
        loadGame(slot);
    } else {
        startNewGame();
        saveGame(slot);
    }
}

void Game::startNewGame() {
    resetGame();
    gameState = Playing;
}

void Game::setPlayerSafePosition(sf::Vector2f position) {
    player.setPosition(position);
    lastSafePlayerPosition = position;
}

void Game::setupTrapdoor() {
    trapdoor.setSize(sf::Vector2f(54.f, 54.f));
    trapdoor.setOrigin(27.f, 27.f);
    trapdoor.setFillColor(sf::Color(55, 35, 20));
    trapdoor.setOutlineThickness(3.f);
    trapdoor.setOutlineColor(sf::Color(110, 80, 45));
    trapdoor.setPosition(576.f, 432.f);
}

void Game::updateTrapdoor() {
    if (!trapdoorActive) {
        return;
    }

    if (trapdoor.getGlobalBounds().intersects(player.getBounds())) {
        trapdoorActive = false;
        progression.emptyTrash();
        gameState = UpgradeMenuState;
        audio.playMenuMusic();
    }
}

void Game::goToNextFloor() {
    //audio.stopBossMusic();
    //audio.playMusic();
    currentFloor++;
    progression.setCurrentFloor(currentFloor);

    trapdoorActive = false;

    chasers.clear();
    shooters.clear();
    throwers.clear();
    bosses.clear();
    medkits.clear();
    messes.clear();

    setupRooms();

    currentRoomX = 1;
    currentRoomY = 1;

    setPlayerSafePosition(sf::Vector2f(576.f, 432.f));

    enterRoom(currentRoomX, currentRoomY);

    spawnChaserClock.restart();
    spawnShooterClock.restart();
    spawnThrowerClock.restart();
    vacuumDamageClock.restart();
    medkitSpawnClock.restart();

    window.setTitle("Piso " + std::to_string(currentFloor));
}

bool Game::isBossRoom(int x, int y) {
    return x == 1 && y == 0;
}

bool Game::areNormalRoomsCleared() {
    for (int y = 0; y < rooms.size(); y++) {
        for (int x = 0; x < rooms[y].size(); x++) {
            if (rooms[y][x].groundFile == "") {
                continue;
            }

            if (isBossRoom(x, y)) {
                continue;
            }

            if (!rooms[y][x].cleared) {
                return false;
            }
        }
    }

    return true;
}

void Game::spawnBoss() {
    bosses.clear();

    int bossCount = 1;

    if (currentFloor >= 5) {
        bossCount = 2;
    }

    if (currentFloor >= 10) {
        bossCount = 3;
    }

    if (currentFloor >= 15) {
        bossCount = 4;
    }

    for (int i = 0; i < bossCount; i++) {
        float x = 576.f;
        float y = 420.f;

        if (bossCount == 2) {
            x = 460.f + i * 230.f;
        } else if (bossCount >= 3) {
            x = 360.f + i * 210.f;
        }

        addBoss(sf::Vector2f(x, y));
    }

    startBossIntro();
}

void Game::startBossIntro() {
    bossIntroClock.restart();

    audio.stopMusic();
    audio.playBossMusic();

    gameState = BossIntroState;
}

void Game::updateBossIntro() {
    if (bossIntroClock.getElapsedTime().asSeconds() >= 3.f) {
        gameState = Playing;
        frameClock.restart();
    }
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = frameClock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        draw();
    }
}

void Game::setupRooms() {
    rooms.resize(3);
    for (int y = 0; y < 3; y++) {
        rooms[y].resize(3);
    }

    rooms[1][1] = {
        "maps/map1-buildingLayer.csv",
        "maps/map1-assetsLayer.csv",
        false,
        false,
        true, true, true, true
    };

    rooms[1][0] = {
        "maps/map2-buildingLayer.csv",
        "maps/map2-assetsLayer.csv",
        false,
        false,
        false, false, false, true
    };

    rooms[0][1] = {
        "maps/map3-buildingLayer.csv",
        "maps/map3-assetsLayer.csv",
        false,
        false,
        false, true, false, false
    };

    rooms[1][2] = {
        "maps/map4-buildingLayer.csv",
        "maps/map4-assetsLayer.csv",
        false,
        false,
        false, false, true, true
    };

    rooms[2][1] = {
        "maps/map5-buildingLayer.csv",
        "maps/map5-assetsLayer.csv",
        false,
        false,
        true, false, false, false
    };
}

void Game::enterRoom(int x, int y) {
    currentRoomX = x;
    currentRoomY = y;

    RoomInfo& room = rooms[currentRoomY][currentRoomX];

    tileMap.loadGroundLayer(room.groundFile);
    tileMap.loadAssetsLayer(room.assetsFile);

    chasers.clear();
    shooters.clear();
    throwers.clear();
    bosses.clear();
    medkits.clear();
    messes.clear();

    if (!room.cleared) {
        if (isBossRoom(currentRoomX, currentRoomY)) {
            spawnBoss();
            window.setTitle("Jefe");
        } else {
            spawnInitialEnemies();
            spawnInitialMess();
        }
    }

    room.visited = true;
}

void Game::checkRoomCleared() {
    RoomInfo& room = rooms[currentRoomY][currentRoomX];

        if (!room.cleared &&
        chasers.empty() &&
        shooters.empty() &&
        throwers.empty() &&
        bosses.empty() &&
        messes.empty()) {
        room.cleared = true;
        window.setTitle("Sala limpia");

        if (isBossRoom(currentRoomX, currentRoomY)) {
            trapdoorActive = true;
        }
    }
}


void Game::checkRoomTransition() {
    RoomInfo& room = rooms[currentRoomY][currentRoomX];

    sf::Vector2f pos = player.getPosition();

    if (pos.x < 0.f) {
        if (room.cleared && room.doorLeft && currentRoomX > 0) {
            enterRoom(currentRoomX - 1, currentRoomY);
            setPlayerSafePosition(sf::Vector2f(1100.f, pos.y));
        } else {
            player.setPosition(sf::Vector2f(20.f, pos.y));
        }
    }

    if (pos.x > 1152.f) {
        if (room.cleared && room.doorRight && currentRoomX < 2) {
            enterRoom(currentRoomX + 1, currentRoomY);
            setPlayerSafePosition(sf::Vector2f(50.f, pos.y));
        } else {
            player.setPosition(sf::Vector2f(1130.f, pos.y));
        }
    }

    if (pos.y < 0.f) {
        int nextX = currentRoomX;
        int nextY = currentRoomY - 1;

        bool canEnterBossRoom = !isBossRoom(nextX, nextY) || areNormalRoomsCleared();

        if (room.cleared &&
            room.doorUp &&
            currentRoomY > 0 &&
            canEnterBossRoom) {
            enterRoom(nextX, nextY);
            setPlayerSafePosition(sf::Vector2f(pos.x, 810.f));
        } else {
            player.setPosition(sf::Vector2f(pos.x, 20.f));
        }
    }

    if (pos.y > 864.f) {
        if (room.cleared && room.doorDown && currentRoomY < 2) {
            enterRoom(currentRoomX, currentRoomY + 1);
            setPlayerSafePosition(sf::Vector2f(pos.x, 50.f));
        } else {
            player.setPosition(sf::Vector2f(pos.x, 840.f));
        }
    }
}
void Game::processEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            continue;
        }

        // Menu principal
        if (gameState == MainMenuState) {
            MenuAction action = mainMenu.handleEvent(event, window);

            if (action == MenuLoadSlot1) {
                openSaveSlot(1);
                audio.playLevelMusic();
            } else if (action == MenuLoadSlot2) {
                openSaveSlot(2);
                audio.playLevelMusic();
            } else if (action == MenuLoadSlot3) {
                openSaveSlot(3);
                audio.playLevelMusic();
            } else if (action == MenuDeleteSlot1) {
                saveSystem.removeSave(1);
            } else if (action == MenuDeleteSlot2) {
                saveSystem.removeSave(2);
            } else if (action == MenuDeleteSlot3) {
                saveSystem.removeSave(3);
            } else if (action == MenuQuit) {
                window.close();
            }

            continue;
        }

        // Menu de mejoras
        if (gameState == UpgradeMenuState) {
            UpgradeAction action =
                upgradeMenu.handleEvent(event, window);

            if (action == UpgradeBuyDamage) {
                progression.buyVacuumDamage();
            } else if (action == UpgradeBuyRange) {
                if (progression.buyVacuumRange()) {
                    aspiradora.setMaxDistance(
                        progression.getVacuumRange()
                    );
                }
            } else if (action == UpgradeBuyCapacity) {
                progression.buyVacuumCapacity();
            } else if (action == UpgradeContinue) {
                goToNextFloor();
                gameState = Playing;
                audio.playLevelMusic();

            }

            continue;
        }

        // Abrir pausa
        if (gameState == Playing &&
            event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            stateBeforePause = gameState;
            gameState = PausedState;
            continue;
        }

        // Menu de pausa
        if (gameState == PausedState) {
            PauseAction action = pauseMenu.handleEvent(event);

            if (action == PauseResume) {
                gameState = stateBeforePause;
                frameClock.restart();
                audio.playLevelMusic();

            } else if (action == PauseSave) {
                activeSaveSlot = pauseMenu.getSelectedSlot();
                saveGame(activeSaveSlot);

            } else if (action == PauseLoad) {
                int slot = pauseMenu.getSelectedSlot();

                if (loadGame(slot)) {
                    activeSaveSlot = slot;
                    audio.playLevelMusic();
                }

            } else if (action == PauseMainMenu) {
                gameState = MainMenuState;

            } else if (action == PauseQuit) {
                window.close();
            }

            continue;
        }
    }
}


void Game::update(float deltaTime) {
    switch (gameState) {
    case MainMenuState:
        break;

    case BossIntroState:
        updateBossIntro();
        break;

    case Playing:
        updatePlaying(deltaTime);
        break;

    case PausedState:
        break;

    case UpgradeMenuState:
        break;

    case GameOver:
        updateGameOver();
        break;
    }
}

void Game::drawBossIntroHud() {
    float time = bossIntroClock.getElapsedTime().asSeconds();
    float percent = time / 3.f;

    if (percent > 1.f) {
        percent = 1.f;
    }

    float barWidth = 520.f;
    float barHeight = 24.f;

    float x = window.getSize().x / 2.f - barWidth / 2.f;
    float y = 28.f;

    sf::RectangleShape back(sf::Vector2f(barWidth, barHeight));
    back.setPosition(x, y);
    back.setFillColor(sf::Color(35, 20, 28));
    back.setOutlineColor(sf::Color(180, 150, 150));
    back.setOutlineThickness(2.f);

    sf::RectangleShape fill(sf::Vector2f(barWidth * percent, barHeight));
    fill.setPosition(x, y);
    fill.setFillColor(sf::Color(160, 20, 45));

    window.draw(back);
    window.draw(fill);

    if (bossFontLoaded && time >= 3.f) {
        sf::FloatRect bounds = bossNameText.getLocalBounds();

        bossNameText.setOrigin(
            bounds.left + bounds.width / 2.f,
            bounds.top + bounds.height / 2.f
        );

        bossNameText.setPosition(window.getSize().x / 2.f, y + 56.f);
        window.draw(bossNameText);
    }
}

void Game::drawBossHealthBar() {
    if (bosses.empty()) {
        return;
    }

    float barWidth = 520.f;
    float barHeight = 24.f;

    float x = window.getSize().x / 2.f - barWidth / 2.f;
    float y = 28.f;

    float percent = bosses[0].getLifePercent();

    sf::RectangleShape back(sf::Vector2f(barWidth, barHeight));
    back.setPosition(x, y);
    back.setFillColor(sf::Color(35, 20, 28));
    back.setOutlineColor(sf::Color(180, 150, 150));
    back.setOutlineThickness(2.f);

    sf::RectangleShape fill(sf::Vector2f(barWidth * percent, barHeight));
    fill.setPosition(x, y);
    fill.setFillColor(sf::Color(160, 20, 45));

    window.draw(back);
    window.draw(fill);

    if (bossFontLoaded) {
        sf::FloatRect bounds = bossNameText.getLocalBounds();

        bossNameText.setOrigin(
            bounds.left + bounds.width / 2.f,
            bounds.top + bounds.height / 2.f
        );

        bossNameText.setPosition(window.getSize().x / 2.f, y + 56.f);
        window.draw(bossNameText);
    }
}

void Game::updatePlaying(float deltaTime) {
    updatePlayer(deltaTime);
    aspiradora.update(window, player.getCenter());

    updateMessCleaning(deltaTime);
    updateEnemies(deltaTime);
    applyVacuumDamage();
    removeDeadEnemies();

    checkRoomCleared();
    checkRoomTransition();
    updateTrapdoor();
    updateMedkits();

    if (player.isDead()) {
    progression.addDeath();

    if (progression.isRunOver()) {
        gameState = GameOver;
        window.setTitle("Perdiste 3 veces - Presiona R para reiniciar");
    } else {
        player.reset();
        player.setPosition(lastSafePlayerPosition);

        window.setTitle(
            "Moriste - Intentos: " +
            std::to_string(progression.getDeaths()) +
            "/" +
            std::to_string(progression.getMaxDeaths())
        );
    }
}
}

void Game::updateGameOver() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        resetGame();
    }
}

void Game::drawTrashBar() {
    float stored = progression.getTrashStored();
    float capacity = progression.getTrashCapacity();

    float percent = 0.f;

    if (capacity > 0.f) {
        percent = stored / capacity;
    }

    if (percent > 1.f) {
        percent = 1.f;
    }

    sf::RectangleShape back(sf::Vector2f(140.f, 12.f));
    back.setPosition(20.f, 70.f);
    back.setFillColor(sf::Color(45, 38, 34));
    back.setOutlineThickness(2.f);
    back.setOutlineColor(sf::Color(120, 100, 80));

    sf::RectangleShape bar(sf::Vector2f(140.f * percent, 12.f));
    bar.setPosition(20.f, 70.f);
    bar.setFillColor(sf::Color(120, 85, 45));

    window.draw(back);
    window.draw(bar);
}

void Game::draw() {

    if (gameState == UpgradeMenuState) {
    upgradeMenu.draw(window, progression);
    window.display();
    return;
}

    if (gameState == MainMenuState) {
    mainMenu.draw(window, saveSystem);
    window.display();
    return;
}
    window.clear(sf::Color::Black);

    tileMap.drawMap(window);

    if (trapdoorActive) {
    window.draw(trapdoor);
}

    for (int i = 0; i < messes.size(); i++) {
        messes[i].draw(window);
    }

    for (int i = 0; i < medkits.size(); i++) {
        medkits[i].draw(window);
    }

    player.drawStamina(window);
    player.drawLife(window);
    drawTrashBar();

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

    for (int i = 0; i < bosses.size(); i++) {
    bosses[i].draw(window);
}

    player.drawStamina(window);
    player.drawLife(window);

    if (gameState == BossIntroState) {
    drawBossIntroHud();
} else if (!bosses.empty()) {
    drawBossHealthBar();
    }

    if (gameState == PausedState) {
    pauseMenu.draw(window, saveSystem);
}

    window.display();
}

void Game::loadMap(int mapNumber) {
    if (mapNumber == 1) {
        tileMap.loadGroundLayer("maps/map1-buildingLayer.csv");
        tileMap.loadAssetsLayer("maps/map1-assetsLayer.csv");
    } else if (mapNumber == 6) {
        tileMap.loadGroundLayer("maps/map6-buildingLayer.csv");
        tileMap.loadAssetsLayer("maps/map6-assetsLayer.csv");
    }
}

void Game::resetGame() {
    player.reset();

    gameState = Playing;

    currentFloor = 1;
    trapdoorActive = false;

    chasers.clear();
    shooters.clear();
    throwers.clear();
    medkits.clear();
    messes.clear();
    bosses.clear();

    setupRooms();
    currentRoomX = 1;
    currentRoomY = 1;
    enterRoom(currentRoomX, currentRoomY);
    setPlayerSafePosition(sf::Vector2f(576.f, 432.f));

    spawnChaserClock.restart();
    spawnShooterClock.restart();
    spawnThrowerClock.restart();
    vacuumDamageClock.restart();
    medkitSpawnClock.restart();

    progression.reset();
    currentFloor = 1;
    trapdoorActive = false;
    aspiradora.setMaxDistance(progression.getVacuumRange());

    window.setTitle("Proyecto Practico");
}

void Game::changeToMap2() {
    currentMap = 2;
    enemiesKilled = 0;
    waitingForCleaning = false;

    loadMap(2);

    chasers.clear();
    shooters.clear();
    throwers.clear();
    medkits.clear();
    messes.clear();

    player.setPosition(sf::Vector2f(100.f, 100.f));

    spawnInitialEnemies();
    spawnInitialMess();

    spawnChaserClock.restart();
    spawnShooterClock.restart();
    spawnThrowerClock.restart();
    medkitSpawnClock.restart();
    vacuumDamageClock.restart();

    window.setTitle("Piso 2");
    audio.playMapChange();
    audio.playLevelMusic();
}

sf::Vector2f Game::randomSpawn() {
    int side = std::rand() % 4;
    float w = (float)window.getSize().x;
    float h = (float)window.getSize().y;

    if (side == 0) return sf::Vector2f((float)(std::rand() % (int)w), -30.f);
    if (side == 1) return sf::Vector2f(w + 30.f, (float)(std::rand() % (int)h));
    if (side == 2) return sf::Vector2f((float)(std::rand() % (int)w), h + 30.f);

    return sf::Vector2f(-30.f, (float)(std::rand() % (int)h));
}

sf::Vector2f Game::randomMessPosition() {
    float x = 60.f + (std::rand() % 680);
    float y = 60.f + (std::rand() % 480);

    return sf::Vector2f(x, y);
}

void Game::spawnInitialEnemies() {
    int chaserCount = 1;
    int shooterCount = 1;
    int throwerCount = 1;

    if (currentFloor >= 2) {
        chaserCount = 2;
    }

    if (currentFloor >= 3) {
        shooterCount = 2;
    }

    if (currentFloor >= 4) {
        throwerCount = 2;
    }

    if (currentFloor >= 5) {
        chaserCount += (currentFloor - 4) / 2;
        shooterCount += (currentFloor - 4) / 3;
        throwerCount += (currentFloor - 4) / 4;
    }

    for (int i = 0; i < chaserCount; i++) {
        addChaser(sf::Vector2f(250.f + i * 90.f, 220.f));
    }

    for (int i = 0; i < shooterCount; i++) {
        addShooter(sf::Vector2f(780.f - i * 90.f, 260.f));
    }

    for (int i = 0; i < throwerCount; i++) {
        addThrower(sf::Vector2f(300.f + i * 120.f, 620.f));
    }
}

void Game::spawnInitialMess() {
    for (int i = 0; i < 12; i++) {
        messes.push_back(mess(randomMessPosition()));
    }
}

void Game::updatePlayer(float deltaTime) {
    sf::Vector2f oldPlayerPosition = player.getPosition();

    player.movement();

    if (tileMap.checkCollision(player.getBounds())) {
        player.setPosition(oldPlayerPosition);
    } else {
        lastSafePlayerPosition = player.getPosition();
    }

    player.update(deltaTime);
}

void Game::updateMessCleaning(float deltaTime) {
    int messCountBefore = messes.size();

    for (int i = 0; i < messes.size(); i++) {
        if (messes[i].getBounds().intersects(aspiradora.getBounds())) {
            messes[i].updateCleaning(deltaTime);
        }
    }

    messes.erase(
        std::remove_if(
            messes.begin(),
            messes.end(),
            [this](mess& dirt) {
                if (!dirt.isCleaned()) {
                    return false;
                }

                float density = dirt.getDensity();

                if (progression.canStoreTrash(density)) {
                    progression.addTrash(density);
                }

                progression.addTrashReward(
                    dirt.getScoreValue(),
                    dirt.getGoldValue()
                );

                return true;
            }
        ),
        messes.end()
    );

    int cleaned = messCountBefore - messes.size();

    if (cleaned > 0) {
        audio.playClean();
    }
}

void Game::updateEnemies(float deltaTime) {
    for (int i = 0; i < chasers.size(); i++) {
        chasers[i].update(deltaTime, player, window);
    }

    for (int i = 0; i < shooters.size(); i++) {
        shooters[i].update(deltaTime, player, aspiradora, window, tileMap);
    }

    for (int i = 0; i < throwers.size(); i++) {
        throwers[i].update(deltaTime, player, aspiradora, window, tileMap);
    }
    for (int i = 0; i < bosses.size(); i++) {
    bosses[i].update(deltaTime, player, aspiradora, window, tileMap);
    }
}

void Game::applyVacuumDamage() {
    if (vacuumDamageClock.getElapsedTime().asSeconds() < 0.25f) {
        return;
    }

    bool hitSomething = false;

    for (int i = 0; i < chasers.size(); i++) {
        if (!chasers[i].isDead() &&
            aspiradora.getBounds().intersects(chasers[i].getBounds())) {
            chasers[i].takeDamage(progression.getVacuumDamage(), aspiradora.getPosition(), window);
            hitSomething = true;
        }
    }

    for (int i = 0; i < shooters.size(); i++) {
        if (!shooters[i].isDead() &&
            aspiradora.getBounds().intersects(shooters[i].getBounds())) {
            shooters[i].takeDamage(progression.getVacuumDamage(), aspiradora.getPosition(), window);
            hitSomething = true;
        }
    }

    for (int i = 0; i < throwers.size(); i++) {
        if (!throwers[i].isDead() &&
            aspiradora.getBounds().intersects(throwers[i].getBounds())) {
            throwers[i].takeDamage(progression.getVacuumDamage(), aspiradora.getPosition(), window);
            hitSomething = true;
        }
    }


    for (int i = 0; i < bosses.size(); i++) {
        if (!bosses[i].isDead() &&
            aspiradora.getBounds().intersects(bosses[i].getBounds())) {
            bosses[i].takeDamage(progression.getVacuumDamage(), aspiradora.getPosition(), window);
            hitSomething = true;
        }
    }

    if (hitSomething) {
        audio.playHit();
        vacuumDamageClock.restart();
}

}

void Game::removeDeadEnemies() {
    for (int i = 0; i < chasers.size(); i++) {
        if (chasers[i].isDead()) {
            messes.push_back(mess(chasers[i].getPosition(), 2));
            chasers.erase(chasers.begin() + i);
            enemiesKilled++;
            progression.addEnemyReward(50, 3);
            i--;
        }
    }

    for (int i = 0; i < shooters.size(); i++) {
        if (shooters[i].isDead()) {
            messes.push_back(mess(shooters[i].getPosition(), 2));
            shooters.erase(shooters.begin() + i);
            enemiesKilled++;
            progression.addEnemyReward(75, 4);
            i--;
        }
    }

    for (int i = 0; i < throwers.size(); i++) {
        if (throwers[i].isDead()) {
            messes.push_back(mess(throwers[i].getPosition(), 2));
            throwers.erase(throwers.begin() + i);
            enemiesKilled++;
            progression.addEnemyReward(100, 5);
            i--;
        }
    }

    for (int i = 0; i < bosses.size(); i++) {
        if (bosses[i].isDead()) {
            messes.push_back(mess(bosses[i].getPosition(), 2));
            bosses.erase(bosses.begin() + i);
            progression.addBossReward(500, 20);
            i--;
    }
}
}

    float Game::getDifficultyMultiplier() {
        return std::pow(1.2f, currentFloor - 1);
    }

    void Game::addChaser(sf::Vector2f position) {
        EnemyChaser enemy(position);
        enemy.applyDifficulty(getDifficultyMultiplier());
        chasers.push_back(enemy);
    }

    void Game::addShooter(sf::Vector2f position) {
        EnemyShooter enemy(position);
        enemy.applyDifficulty(getDifficultyMultiplier());
        shooters.push_back(enemy);
    }

    void Game::addThrower(sf::Vector2f position) {
        EnemyThrower enemy(position);
        enemy.applyDifficulty(getDifficultyMultiplier());
        throwers.push_back(enemy);
    }

    void Game::addBoss(sf::Vector2f position) {
        EnemyBoss boss(position);
        boss.applyDifficulty(getDifficultyMultiplier());
        bosses.push_back(boss);
}

void Game::checkMapProgress() {
    if (enemiesKilled >= enemiesNeededForNextMap) {
        waitingForCleaning = true;
    }
}

void Game::updateSpawns() {
    RoomInfo& room = rooms[currentRoomY][currentRoomX];

    if (room.cleared || isBossRoom(currentRoomX, currentRoomY)) {
        return;
    }

    int maxChasers = 1 + currentFloor;
    int maxShooters = 1 + currentFloor / 2;
    int maxThrowers = 1 + currentFloor / 3;

    if (spawnShooterClock.getElapsedTime().asSeconds() >= 6.f &&
        shooters.size() < maxShooters) {
        addShooter(randomSpawn());
        spawnShooterClock.restart();
    }

    if (spawnChaserClock.getElapsedTime().asSeconds() >= 6.f &&
        chasers.size() < maxChasers) {
        addChaser(randomSpawn());
        spawnChaserClock.restart();
    }

    if (spawnThrowerClock.getElapsedTime().asSeconds() >= 8.f &&
        throwers.size() < maxThrowers) {
        addThrower(randomSpawn());
        spawnThrowerClock.restart();
    }
}

void Game::updateMedkits() {
    if (medkitSpawnClock.getElapsedTime().asSeconds() >= 5.f && medkits.size() < 3) {
        float x = 60.f + (std::rand() % 680);
        float y = 60.f + (std::rand() % 480);

        medkits.push_back(Medkit(sf::Vector2f(x, y)));
        medkitSpawnClock.restart();
    }

    medkits.erase(
        std::remove_if(
            medkits.begin(),
            medkits.end(),
            [this](Medkit& medkit) {
                if (medkit.getBounds().intersects(player.getBounds())) {
                    player.heal(1);
                    audio.playMedkit();
                    return true;
                }

                return false;
            }
        ),
        medkits.end()
    );
}

void Game::checkMapChange() {
    if (waitingForCleaning &&
        messes.empty() &&
        chasers.empty() &&
        shooters.empty() &&
        throwers.empty() &&
        currentMap == 1) {
        changeToMap2();
    }
}


