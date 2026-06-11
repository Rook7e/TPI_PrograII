#include "AudioManager.h"
#include <iostream>

bool AudioManager::load() {
    bool ok = true;

    if (!music.openFromFile("assets/Sonidos/music.wav")) {
        std::cout << "Error al cargar assets/music.wav" << std::endl;
        ok = false;
    }

    if (!hitBuffer.loadFromFile("assets/Sonidos/hit.wav")) {
        std::cout << "Error al cargar assets/hit.wav" << std::endl;
        ok = false;
    }

    if (!cleanBuffer.loadFromFile("assets/Sonidos/clean.wav")) {
        std::cout << "Error al cargar assets/clean.wav" << std::endl;
        ok = false;
    }

    if (!medkitBuffer.loadFromFile("assets/Sonidos/medkit.wav")) {
        std::cout << "Error al cargar assets/medkit.wav" << std::endl;
        ok = false;
    }

    if (!mapChangeBuffer.loadFromFile("assets/Sonidos/mapchange.wav")) {
        std::cout << "Error al cargar assets/mapchange.wav" << std::endl;
        ok = false;
    }

    hitSound.setBuffer(hitBuffer);
    cleanSound.setBuffer(cleanBuffer);
    medkitSound.setBuffer(medkitBuffer);
    mapChangeSound.setBuffer(mapChangeBuffer);

    music.setLoop(true);
    music.setVolume(5.f);

    hitSound.setVolume(60.f);
    cleanSound.setVolume(45.f);
    medkitSound.setVolume(70.f);
    mapChangeSound.setVolume(80.f);

    return ok;
}

void AudioManager::playMusic() {
    music.play();
}

void AudioManager::stopMusic() {
    music.stop();
}

void AudioManager::playHit() {
    hitSound.play();
}

void AudioManager::playClean() {
    cleanSound.play();
}

void AudioManager::playMedkit() {
    medkitSound.play();
}

void AudioManager::playMapChange() {
    mapChangeSound.play();
}
