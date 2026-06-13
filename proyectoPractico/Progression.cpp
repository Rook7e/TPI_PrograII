#include "Progression.h"

Progression::Progression() {
    reset();
}

void Progression::reset() {
    score = 0;
    gold = 0;
    deaths = 0;
    maxDeaths = 3;
    currentFloor = 1;

    vacuumDamageLevel = 0;
    vacuumRangeLevel = 0;
    vacuumCapacityLevel = 0;

    trashStored = 0.f;
}

void Progression::addScore(int amount) {
    score += amount;
}

void Progression::addGold(int amount) {
    gold += amount;
}

float Progression::getFloorMultiplier() {
    return 1.f + (currentFloor - 1) * 0.15f;
}

void Progression::addEnemyReward(int baseScore, int baseGold) {
    float multiplier = getFloorMultiplier();

    score += (int)(baseScore * multiplier);
    gold += (int)(baseGold * multiplier);
}

void Progression::addTrashReward(int baseScore, int baseGold) {
    float multiplier = getFloorMultiplier();

    score += (int)(baseScore * multiplier);
    gold += (int)(baseGold * multiplier);
}

void Progression::addBossReward(int baseScore, int baseGold) {
    float multiplier = getFloorMultiplier();

    score += (int)(baseScore * multiplier);
    gold += (int)(baseGold * multiplier);
}

int Progression::getScore() {
    return score;
}

int Progression::getGold() {
    return gold;
}

int Progression::getDeaths() {
    return deaths;
}

int Progression::getMaxDeaths() {
    return maxDeaths;
}

void Progression::addDeath() {
    deaths++;
}

bool Progression::isRunOver() {
    return deaths >= maxDeaths;
}

int Progression::getCurrentFloor() {
    return currentFloor;
}

void Progression::setCurrentFloor(int floor) {
    currentFloor = floor;
}

int Progression::getVacuumDamageCost() {
    return 10 + vacuumDamageLevel * 8;
}

int Progression::getVacuumRangeCost() {
    return 8 + vacuumRangeLevel * 6;
}

bool Progression::buyVacuumDamage() {
    int cost = getVacuumDamageCost();

    if (gold < cost) {
        return false;
    }

    gold -= cost;
    vacuumDamageLevel++;

    return true;
}

bool Progression::buyVacuumRange() {
    int cost = getVacuumRangeCost();

    if (gold < cost) {
        return false;
    }

    gold -= cost;
    vacuumRangeLevel++;

    return true;
}

int Progression::getVacuumDamage() {
    return 1 + vacuumDamageLevel;
}

float Progression::getVacuumRange() {
    return 120.f + vacuumRangeLevel * 25.f;
}

void Progression::addRewards(int amount) {
    addGold(amount);
}

int Progression::getRewards() {
    return getGold();
}

int Progression::getVacuumDamageLevel() {
    return vacuumDamageLevel;
}

int Progression::getVacuumRangeLevel() {
    return vacuumRangeLevel;
}

float Progression::getTrashStored() {
    return trashStored;
}

float Progression::getTrashCapacity() {
    return 20.f + vacuumCapacityLevel * 10.f;
}

bool Progression::canStoreTrash(float amount) {
    return trashStored + amount <= getTrashCapacity();
}

void Progression::addTrash(float amount) {
    trashStored += amount;

    if (trashStored > getTrashCapacity()) {
        trashStored = getTrashCapacity();
    }
}

void Progression::emptyTrash() {
    trashStored = 0.f;
}

int Progression::getVacuumCapacityCost() {
    return 12 + vacuumCapacityLevel * 8;
}

bool Progression::buyVacuumCapacity() {
    int cost = getVacuumCapacityCost();

    if (gold < cost) {
        return false;
    }

    gold -= cost;
    vacuumCapacityLevel++;

    return true;
}

int Progression::getVacuumCapacityLevel() {
    return vacuumCapacityLevel;
}
