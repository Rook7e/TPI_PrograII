#include "AudioManager.h"
#include <iostream>

bool AudioManager::load() {
    bool ok = true;

    // Cargamos sonidos cortos en memoria usando SoundBuffer.
    // Estos son efectos de sonido, no canciones largas.
    if (!hitBuffer.loadFromFile("assets/Sonidos/hit.wav")) {
        std::cout << "Error al cargar assets/Sonidos/hit.wav" << std::endl;
        ok = false;
    }

    if (!cleanBuffer.loadFromFile("assets/Sonidos/clean.wav")) {
        std::cout << "Error al cargar assets/Sonidos/clean.wav" << std::endl;
        ok = false;
    }

    if (!medkitBuffer.loadFromFile("assets/Sonidos/medkit.wav")) {
        std::cout << "Error al cargar assets/Sonidos/medkit.wav" << std::endl;
        ok = false;
    }

    if (!mapChangeBuffer.loadFromFile("assets/Sonidos/mapchange.wav")) {
        std::cout << "Error al cargar assets/Sonidos/mapchange.wav" << std::endl;
        ok = false;
    }

    // Asociamos cada buffer con su sonido.
    // El sf::Sound reproduce el audio que tiene asignado como buffer.
    hitSound.setBuffer(hitBuffer);
    cleanSound.setBuffer(cleanBuffer);
    medkitSound.setBuffer(medkitBuffer);
    mapChangeSound.setBuffer(mapChangeBuffer);

    // Volumen de cada efecto.
    // SFML usa valores de 0 a 100.
    hitSound.setVolume(5.f);
    cleanSound.setVolume(45.f);
    medkitSound.setVolume(70.f);
    mapChangeSound.setVolume(80.f);

    return ok;
}

void AudioManager::playMenuMusic() {
    // Frenamos la musica anterior antes de abrir otra.
    music.stop();

    // sf::Music se usa para canciones largas porque reproduce por streaming.
    if (music.openFromFile("assets/Sonidos/menu.ogg")) {
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

    if (music.openFromFile("assets/Sonidos/boss.ogg")) {
        music.setLoop(true);
        music.setVolume(40.f);
        music.play();
    }
}

void AudioManager::stopMusic() {
    music.stop();
}

void AudioManager::stopBossMusic() {
    // Quedo por compatibilidad con versiones anteriores.
    // Actualmente la musica del jefe usa el mismo objeto music.
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
