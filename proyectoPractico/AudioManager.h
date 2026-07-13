#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#pragma once
#include <SFML/Audio.hpp>

// Clase encargada de centralizar todo el audio del juego.
// Maneja musica de menu, musica de nivel, musica de jefe y efectos cortos.
class AudioManager {
private:
    // Usamos sf::Music para canciones largas porque SFML las reproduce por streaming.
    sf::Music music;
    sf::Music bossMusic;

    // Buffers para sonidos cortos. El buffer guarda el archivo de sonido en memoria.
    sf::SoundBuffer hitBuffer;
    sf::SoundBuffer cleanBuffer;
    sf::SoundBuffer medkitBuffer;
    sf::SoundBuffer mapChangeBuffer;

    // Objetos de sonido que reproducen los buffers.
    sf::Sound hitSound;
    sf::Sound cleanSound;
    sf::Sound medkitSound;
    sf::Sound mapChangeSound;

public:
    // Carga todos los efectos de sonido necesarios.
    bool load();

    // Cambian la musica segun el estado del juego.
    void playMenuMusic();
    void playLevelMusic();
    void playBossMusic();
    void stopMusic();

    // Mantuvimos esta funcion por compatibilidad con versiones anteriores.
    void stopBossMusic();

    // Efectos de sonido del gameplay.
    void playHit();
    void playClean();
    void playMedkit();
    void playMapChange();
};

#endif
