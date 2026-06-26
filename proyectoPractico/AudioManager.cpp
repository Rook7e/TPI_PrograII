#include "AudioManager.h"
#include <iostream>

bool AudioManager::load() {
    bool ok = true;

    // Solo cargamos los buffers de los efectos de sonido de corta duraci�n (SFX)
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

    // Vinculamos los buffers a sus respectivos objetos de sonido
    hitSound.setBuffer(hitBuffer);
    cleanSound.setBuffer(cleanBuffer);
    medkitSound.setBuffer(medkitBuffer);
    mapChangeSound.setBuffer(mapChangeBuffer);

    // Ajustes de volumen razonables (SFML maneja rango de 0 a 100)
    hitSound.setVolume(80.f);
    cleanSound.setVolume(45.f);
    medkitSound.setVolume(70.f);
    mapChangeSound.setVolume(80.f);

    return ok;
}

// --- M�todos de reproducci�n de m�sica ---

void AudioManager::playMenuMusic() {
    music.stop();
    if (music.openFromFile("assets/Sonidos/menu.ogg")) { // Asegurate que exista este archivo
        music.setLoop(true);
        music.setVolume(30.f);
        music.play();
    }
}

void AudioManager::playLevelMusic() {
    music.stop();
    if (music.openFromFile("assets/Sonidos/music.ogg")) {
        music.setLoop(true);
        music.setVolume(30.f);
        music.play();
    }
}

void AudioManager::playBossMusic() {
    music.stop();
    if (music.openFromFile("assets/Sonidos/boss.ogg")) { // Asegurate que exista este archivo
        music.setLoop(true);
        music.setVolume(40.f);
        music.play();
    }
}

void AudioManager::stopMusic() {
    music.stop();
}

// --- M�todos de reproducci�n de efectos (SFX) ---

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
