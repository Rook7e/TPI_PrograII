#include "Game.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

Game::Game()
    : window(sf::VideoMode(800, 600), "Proyecto Practico")
{
    window.setFramerateLimit(60);
    audio.load();
    audio.playMusic();
    std::srand((unsigned)std::time(NULL));

    if (!tileMap.load()) {
        window.close();
    }

    gameState = Playing;
    currentMap = 1;
    enemiesKilled = 0;
    enemiesNeededForNextMap = 6;
    waitingForCleaning = false;

    loadMap(1);
    spawnInitialEnemies();
    spawnInitialMess();
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = frameClock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        draw();
    }
}

void Game::processEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update(float deltaTime) {
    switch (gameState) {
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

    checkMapProgress();
    updateSpawns();
    checkMapChange();
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

void Game::loadMap(int mapNumber) {
    if (mapNumber == 1) {
        tileMap.loadGroundLayer("maps/map1-groundLayer.csv");
        tileMap.loadAssetsLayer("maps/map1-assetsLayer.csv");
    } else if (mapNumber == 2) {
        tileMap.loadGroundLayer("maps/map2-groundLayer.csv");
        tileMap.loadAssetsLayer("maps/map2-assetsLayer.csv");
    }
}

void Game::resetGame() {
    player.reset();

    currentMap = 1;
    enemiesKilled = 0;
    waitingForCleaning = false;
    gameState = Playing;

    loadMap(1);

    chasers.clear();
    shooters.clear();
    throwers.clear();
    medkits.clear();
    messes.clear();

    spawnInitialEnemies();
    spawnInitialMess();

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

    window.setTitle("Mapa 2");
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

    throwers.erase(
        std::remove_if(
            throwers.begin(),
            throwers.end(),
            [this](EnemyThrower& enemy) {
                if (enemy.isDead()) {
                    enemiesKilled++;
                    return true;
                }

                return false;
            }
        ),
        throwers.end()
    );
}

void Game::checkMapProgress() {
    if (enemiesKilled >= enemiesNeededForNextMap) {
        waitingForCleaning = true;
    }
}

void Game::updateSpawns() {
    if (!waitingForCleaning &&
        spawnShooterClock.getElapsedTime().asSeconds() >= 6.f &&
        shooters.size() < 3) {
        shooters.push_back(EnemyShooter(randomSpawn()));
        spawnShooterClock.restart();
    }

    if (!waitingForCleaning &&
        spawnChaserClock.getElapsedTime().asSeconds() >= 6.f &&
        chasers.size() < 5) {
        chasers.push_back(EnemyChaser(randomSpawn()));
        spawnChaserClock.restart();
    }

    if (!waitingForCleaning &&
        spawnThrowerClock.getElapsedTime().asSeconds() >= 8.f &&
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
