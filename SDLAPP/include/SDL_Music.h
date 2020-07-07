#ifndef SDL_MUSIC_H
#define SDL_MUSIC_H
#include<string>
#include<SDL_mixer.h>
class SDL_Music
{
public:
    int m_volume=100;
    bool IsItPlay = false;
    bool IsRepeat = true;
    SDL_Music(std::string wavfile);
    virtual ~SDL_Music();
    float GetCurrentPogress();
    int PlayIt();
    void PauseIt();
    void StopMusic();
    void UpdateVolume();
protected:
    Mix_Chunk *sample;
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_AudioDeviceID deviceId = 0;
    friend void fill_audio(void *udata, Uint8 *stream, int len);
private:
};

#endif // SDL_MUSIC_H
