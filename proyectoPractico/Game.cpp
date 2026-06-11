#include "Game.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game()
    : window(sf::VideoMode(1152, 864), "Proyecto Practico")
{
    window.setFramerateLimit(60);
    audio.load();
    audio.playMusic();
    std::srand((unsigned)std::time(NULL));

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
        goToNextFloor();
    }
}

void Game::goToNextFloor() {
    currentFloor++;

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

    player.setPosition(sf::Vector2f(576.f, 432.f));

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
    bosses.push_back(EnemyBoss(sf::Vector2f(576.f, 360.f)));
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
            player.setPosition(sf::Vector2f(1100.f, pos.y));
        } else {
            player.setPosition(sf::Vector2f(20.f, pos.y));
        }
    }

    if (pos.x > 1152.f) {
        if (room.cleared && room.doorRight && currentRoomX < 2) {
            enterRoom(currentRoomX + 1, currentRoomY);
            player.setPosition(sf::Vector2f(50.f, pos.y));
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
            player.setPosition(sf::Vector2f(pos.x, 810.f));
        } else {
            player.setPosition(sf::Vector2f(pos.x, 20.f));
        }
    }

    if (pos.y > 864.f) {
        if (room.cleared && room.doorDown && currentRoomY < 2) {
            enterRoom(currentRoomX, currentRoomY + 1);
            player.setPosition(sf::Vector2f(pos.x, 50.f));
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
            }

        if (gameState == MainMenuState) {
        MenuAction action = mainMenu.handleEvent(event, window);

        if (action == MenuStart) {
            gameState = Playing;
        } else if (action == MenuQuit) {
            window.close();
        }
    }

    }
}

void Game::update(float deltaTime) {
    switch (gameState) {

    case MainMenuState:
         break;

    case Playing:
        updatePlaying(deltaTime);
        break;

    case GameOver:
        updateGameOver();
        break;
    }
}

void Game::updatePlaying(float deltaTime) {
    updatePlayer(deltaTime);
    aspiradora.update(window, player.getCenter());

    updateMessCleaning();
    updateEnemies(deltaTime);
    applyVacuumDamage();
    removeDeadEnemies();

    checkRoomCleared();
    checkRoomTransition();
    updateTrapdoor();
    updateMedkits();

    if (player.isDead()) {
        gameState = GameOver;
        window.setTitle("Estas muerto - Presiona R para reiniciar");
    }
}

void Game::updateGameOver() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        resetGame();
    }
}

void Game::draw() {

    if (gameState == MainMenuState) {
    mainMenu.draw(window);
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

    spawnChaserClock.restart();
    spawnShooterClock.restart();
    spawnThrowerClock.restart();
    vacuumDamageClock.restart();
    medkitSpawnClock.restart();

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
    chasers.push_back(EnemyChaser(sf::Vector2f(700.f, 100.f)));


    shooters.push_back(EnemyShooter(sf::Vector2f(650.f, 500.f)));


    throwers.push_back(EnemyThrower(sf::Vector2f(200.f, 500.f)));
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
    }

    player.update(deltaTime);
}

void Game::updateMessCleaning() {
    int messCountBefore = messes.size();

    messes.erase(
        std::remove_if(
            messes.begin(),
            messes.end(),
            [this](mess& dirt) {
                return dirt.getBounds().intersects(aspiradora.getBounds());
            }
        ),
        messes.end()
    );

    if (messes.size() < messCountBefore) {
        audio.playClean();
    }
}

void Game::updateEnemies(float deltaTime) {
    for (int i = 0; i < chasers.size(); i++) {
        chasers[i].update(deltaTime, player);
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


    for (int i = 0; i < bosses.size(); i++) {
        if (!bosses[i].isDead() &&
            aspiradora.getBounds().intersects(bosses[i].getBounds())) {
            bosses[i].takeDamage(1, aspiradora.getPosition());
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

    for (int i = 0; i < throwers.size(); i++) {
        if (throwers[i].isDead()) {
            messes.push_back(mess(throwers[i].getPosition(), 2));
            throwers.erase(throwers.begin() + i);
            enemiesKilled++;
            i--;
        }
    }

    for (int i = 0; i < bosses.size(); i++) {
        if (bosses[i].isDead()) {
            messes.push_back(mess(bosses[i].getPosition(), 2));
            bosses.erase(bosses.begin() + i);
            i--;
    }
}
}

void Game::checkMapProgress() {
    if (enemiesKilled >= enemiesNeededForNextMap) {
        waitingForCleaning = true;
    }
}

void Game::updateSpawns() {
    RoomInfo& room = rooms[currentRoomY][currentRoomX];

    if (room.cleared) {
        return;
    }

    if (spawnShooterClock.getElapsedTime().asSeconds() >= 6.f &&
        shooters.size() < 3) {
        shooters.push_back(EnemyShooter(randomSpawn()));
        spawnShooterClock.restart();
    }

    if (spawnChaserClock.getElapsedTime().asSeconds() >= 6.f &&
        chasers.size() < 5) {
        chasers.push_back(EnemyChaser(randomSpawn()));
        spawnChaserClock.restart();
    }

    if (spawnThrowerClock.getElapsedTime().asSeconds() >= 8.f &&
        throwers.size() < 2) {
        throwers.push_back(EnemyThrower(randomSpawn()));
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
