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

    void playMusic();
    void stopMusic();

    void playHit();
    void playClean();
    void playMedkit();
    void playMapChange();
};

#endif
