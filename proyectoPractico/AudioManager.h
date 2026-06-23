#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#pragma once
#include <SFML/Audio.hpp>

class AudioManager {
private:
    sf::Music music;

    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer cleanBuffer;
    sf::SoundBuffer medkitBuffer;
    sf::SoundBuffer mapChangeBuffer;

    sf::Sound hitSound;
    sf::Sound cleanSound;
    sf::Sound medkitSound;
    sf::Sound mapChangeSound;

public:
    bool load();

    // Funciones controladoras de música para las instancias
    void playMenuMusic();  // Para MenuPrincipal y Tienda
    void playLevelMusic(); // Para el juego normal
    void playBossMusic();  // Para la pelea con el Jefe
    void stopMusic();      // Para frenar limpiamente

    // Efectos de sonido (SFX)
    void playHit();
    void playClean();
    void playMedkit();
    void playMapChange();
};

#endif
