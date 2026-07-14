#ifndef SAVESYSTEM_H_INCLUDED
#define SAVESYSTEM_H_INCLUDED

#include <string>

// Estructura con todos los datos que se guardan en una partida.
// Es simple a proposito para poder escribirla y leerla en binario.
struct SaveData {
    // Progreso general.
    int score;
    int gold;
    int deaths;
    int currentFloor;

    // Mejoras compradas.
    int vacuumDamageLevel;
    int vacuumRangeLevel;
    int vacuumCapacityLevel;

    // Basura acumulada.
    float trashStored;

    // Sala actual.
    int currentRoomX;
    int currentRoomY;

    // Posicion del jugador.
    float playerX;
    float playerY;

    // Estado de salas limpias. Son 9 porque el mapa de salas es 3x3.
    bool roomsCleared[9];

    // Cantidad de enemigos vivos al guardar.
    int chaserCount;
    int shooterCount;
    int throwerCount;
    int bossCount;
};

// Sistema de guardado por ranuras.
// Guarda/carga archivos binarios dentro de la carpeta saves.
class SaveSystem {
public:
    SaveSystem();

    // Guarda datos en una ranura.
    bool save(int slot, const SaveData& data) const;

    // Carga datos desde una ranura.
    bool load(int slot, SaveData& data) const;

    // Indica si una ranura tiene partida valida.
    bool exists(int slot) const;

    // Devuelve texto resumido para mostrar en menus.
    std::string getSummary(int slot) const;

    // Borra una partida guardada.
    bool removeSave(int slot) const;

private:
    // Construye la ruta del archivo segun la ranura.
    std::string getPath(int slot) const;
};

#endif
