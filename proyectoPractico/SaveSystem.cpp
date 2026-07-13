#include "SaveSystem.h"
#include <cstdint>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <cstdio>

// Namespace anonimo: todo lo que esta aca solo existe en este archivo.
// Sirve para constantes internas del sistema de guardado.
namespace {
const std::uint32_t SAVE_MAGIC = 0x56414331; // "VAC1"
const std::uint32_t SAVE_VERSION = 2;

// Estructura real que se escribe en el archivo.
// Incluye una firma, una version y los datos de la partida.
struct SaveFile {
    std::uint32_t magic;
    std::uint32_t version;
    SaveData data;
};
}

SaveSystem::SaveSystem() {
    // Crea la carpeta saves si no existe.
    // _mkdir es de Windows.
    _mkdir("saves");
}

std::string SaveSystem::getPath(int slot) const {
    // Solo permitimos 3 ranuras.
    if (slot < 1 || slot > 3) {
        return "";
    }

    return "saves/slot" + std::to_string(slot) + ".bin";
}

bool SaveSystem::save(int slot, const SaveData& data) const {
    const std::string path = getPath(slot);

    if (path.empty()) {
        return false;
    }

    // Armamos el archivo con firma, version y datos.
    SaveFile file = {};
    file.magic = SAVE_MAGIC;
    file.version = SAVE_VERSION;
    file.data = data;

    // Abrimos en binario para guardar la estructura completa.
    std::ofstream output(path.c_str(), std::ios::binary);

    if (!output) {
        return false;
    }

    output.write(reinterpret_cast<const char*>(&file), sizeof(file));

    return output.good();
}

bool SaveSystem::load(int slot, SaveData& data) const {
    const std::string path = getPath(slot);

    if (path.empty()) {
        return false;
    }

    SaveFile file = {};

    // Abrimos el archivo binario.
    std::ifstream input(path.c_str(), std::ios::binary);

    if (!input) {
        return false;
    }

    input.read(reinterpret_cast<char*>(&file), sizeof(file));

    // Validamos que sea un guardado nuestro y de la version actual.
    if (!input || file.magic != SAVE_MAGIC || file.version != SAVE_VERSION) {
        return false;
    }

    data = file.data;

    return true;
}

bool SaveSystem::exists(int slot) const {
    // Si se puede cargar, entonces existe y es valido.
    SaveData data = {};
    return load(slot, data);
}

std::string SaveSystem::getSummary(int slot) const {
    SaveData data = {};

    // Texto que se muestra en el menu si no hay partida.
    if (!load(slot, data)) {
        return "Ranura " + std::to_string(slot) + " - Vacia";
    }

    // Texto resumen para el menu principal y pausa.
    std::ostringstream text;
    text << "Ranura " << slot
         << " - Piso " << data.currentFloor
         << " | Oro " << data.gold
         << " | Score " << data.score;

    return text.str();
}

bool SaveSystem::removeSave(int slot) const {
    const std::string path = getPath(slot);

    if (path.empty()) {
        return false;
    }

    // Borra el archivo del slot.
    return std::remove(path.c_str()) == 0;
}
