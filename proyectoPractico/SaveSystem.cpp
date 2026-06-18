#include "SaveSystem.h"
#include <cstdint>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <cstdio>

namespace {
const std::uint32_t SAVE_MAGIC = 0x56414331; // "VAC1"
const std::uint32_t SAVE_VERSION = 2;

struct SaveFile {
    std::uint32_t magic;
    std::uint32_t version;
    SaveData data;
};
}

SaveSystem::SaveSystem() {
    _mkdir("saves");
}

std::string SaveSystem::getPath(int slot) const {
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

    SaveFile file = {};
    file.magic = SAVE_MAGIC;
    file.version = SAVE_VERSION;
    file.data = data;

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
    std::ifstream input(path.c_str(), std::ios::binary);
    if (!input) {
        return false;
    }

    input.read(reinterpret_cast<char*>(&file), sizeof(file));

    if (!input || file.magic != SAVE_MAGIC || file.version != SAVE_VERSION) {
        return false;
    }

    data = file.data;
    return true;
}

bool SaveSystem::exists(int slot) const {
    SaveData data = {};
    return load(slot, data);
}

std::string SaveSystem::getSummary(int slot) const {
    SaveData data = {};

    if (!load(slot, data)) {
        return "Ranura " + std::to_string(slot) + " - Vacia";
    }

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

    return std::remove(path.c_str()) == 0;
}
