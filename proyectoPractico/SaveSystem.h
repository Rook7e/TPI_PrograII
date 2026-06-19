#ifndef SAVESYSTEM_H_INCLUDED
#define SAVESYSTEM_H_INCLUDED

#include <string>

struct SaveData {
    int score;
    int gold;
    int deaths;
    int currentFloor;

    int vacuumDamageLevel;
    int vacuumRangeLevel;
    int vacuumCapacityLevel;
    float trashStored;

    int currentRoomX;
    int currentRoomY;

    float playerX;
    float playerY;

    bool roomsCleared[9];
    int chaserCount;
    int shooterCount;
    int throwerCount;
    int bossCount;
};

class SaveSystem {
public:
    SaveSystem();

    bool save(int slot, const SaveData& data) const;
    bool load(int slot, SaveData& data) const;
    bool exists(int slot) const;
    std::string getSummary(int slot) const;
    bool removeSave(int slot) const;

private:
    std::string getPath(int slot) const;
};

#endif
