#ifndef PROGRESSION_H_INCLUDED
#define PROGRESSION_H_INCLUDED

class Progression {
private:
    int score;
    int gold;
    int deaths;
    int maxDeaths;
    int currentFloor;

    int vacuumDamageLevel;
    int vacuumRangeLevel;
    int vacuumCapacityLevel;

    float trashStored;

public:
    Progression();

    void reset();

    void addScore(int amount);
    void addGold(int amount);

    void addEnemyReward(int baseScore, int baseGold);
    void addTrashReward(int baseScore, int baseGold);
    void addBossReward(int baseScore, int baseGold);

    int getScore();
    int getGold();

    int getDeaths();
    int getMaxDeaths();
    void addDeath();
    bool isRunOver();

    int getCurrentFloor();
    void setCurrentFloor(int floor);
    float getFloorMultiplier();

    int getVacuumDamageCost();
    int getVacuumRangeCost();

    bool buyVacuumDamage();
    bool buyVacuumRange();

    int getVacuumDamage();
    float getVacuumRange();

    int getVacuumDamageLevel();
    int getVacuumRangeLevel();

    float getTrashStored();
    float getTrashCapacity();
    bool canStoreTrash(float amount);
    void addTrash(float amount);
    void emptyTrash();

    int getVacuumCapacityCost();
    bool buyVacuumCapacity();
    int getVacuumCapacityLevel();

    void addRewards(int amount);
    int getRewards();
};

#endif
