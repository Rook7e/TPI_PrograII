#ifndef PROGRESSION_H_INCLUDED
#define PROGRESSION_H_INCLUDED

// Clase que guarda el progreso general de la partida.
// Maneja score, oro, muertes, piso actual, mejoras y capacidad de basura.
class Progression {
private:
    // Puntos y moneda.
    int score;
    int gold;

    // Sistema de vidas/derrotas de la run.
    int deaths;
    int maxDeaths;

    // Piso actual.
    int currentFloor;

    // Niveles de mejoras compradas.
    int vacuumDamageLevel;
    int vacuumRangeLevel;
    int vacuumCapacityLevel;

    // Basura acumulada en la aspiradora.
    float trashStored;

public:
    Progression();

    // Reinicia toda la progresion.
    void reset();

    // Suma directa de score/oro.
    void addScore(int amount);
    void addGold(int amount);

    // Recompensas con multiplicador segun piso.
    void addEnemyReward(int baseScore, int baseGold);
    void addTrashReward(int baseScore, int baseGold);
    void addBossReward(int baseScore, int baseGold);

    // Getters de score/oro.
    int getScore();
    int getGold();

    // Sistema de derrotas.
    int getDeaths();
    int getMaxDeaths();
    void addDeath();
    bool isRunOver();

    // Piso actual y multiplicador de recompensas.
    int getCurrentFloor();
    void setCurrentFloor(int floor);
    float getFloorMultiplier();

    // Costos de mejoras.
    int getVacuumDamageCost();
    int getVacuumRangeCost();

    // Compra de mejoras.
    bool buyVacuumDamage();
    bool buyVacuumRange();

    // Valores reales que usa el gameplay.
    int getVacuumDamage();
    float getVacuumRange();

    // Niveles para mostrar en el menu.
    int getVacuumDamageLevel();
    int getVacuumRangeLevel();

    // Capacidad de basura de la aspiradora.
    float getTrashStored();
    float getTrashCapacity();
    bool canStoreTrash(float amount);
    void addTrash(float amount);
    void emptyTrash();

    // Mejora de capacidad.
    int getVacuumCapacityCost();
    bool buyVacuumCapacity();
    int getVacuumCapacityLevel();

    // Compatibilidad con sistema anterior de recompensas.
    void addRewards(int amount);
    int getRewards();

    // Carga datos desde una partida guardada.
    void loadState(
        int newScore,
        int newGold,
        int newDeaths,
        int newFloor,
        int damageLevel,
        int rangeLevel,
        int capacityLevel,
        float storedTrash
    );
};

#endif
