#include "SDL_Music.h"

static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len =0; // remaining length of the sample we have to play

void fill_audio(void *udata, Uint8 *stream, int len)
{
    /* Only play if we have data left */
    if (!((SDL_Music*)udata)->IsRepeat && audio_len == 0 )
    {
        ((SDL_Music*)udata)->StopMusic();
        return;
    }
    else if(((SDL_Music*)udata)->IsRepeat && audio_len == 0)
    {
        audio_len = ((SDL_Music*)udata)->wavLength;
        audio_pos = ((SDL_Music*)udata)->wavBuffer;
    }

    SDL_memset(stream, 0, len);
    /* Mix as much data as possible */
    len = ( len > audio_len? audio_len : len );
    SDL_MixAudioFormat(stream,audio_pos,((SDL_Music*)udata)->wavSpec.format
        , len, ((SDL_Music*)udata)->m_volume*SDL_MIX_MAXVOLUME/100);
    audio_pos += len;
    audio_len -= len;
}


SDL_Music::SDL_Music(std::string wavfile)
{

    SDL_LoadWAV(wavfile.c_str(), &wavSpec, &wavBuffer, &wavLength);
    wavSpec.callback=fill_audio;
    wavSpec.userdata=this;

    //deviceId = SDL_OpenAudio(&wavSpec, NULL);
    audio_len = wavLength;
    audio_pos = wavBuffer;
}

SDL_Music::~SDL_Music()
{
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

float SDL_Music::GetCurrentPogress()
{
    return (float)(wavLength-audio_len)/wavLength;
}

int SDL_Music::PlayIt()
{
    int result;
    if(audio_len==0)
    {
        audio_len = wavLength;
        audio_pos = wavBuffer;
    }else if(deviceId==0){
        deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if(deviceId==0) printf("OpenAudioDevice Error!!!");
        result = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    }
    IsItPlay = true;
    SDL_PauseAudioDevice(deviceId, 0);
    return result;
}

void SDL_Music::PauseIt()
{
    SDL_PauseAudioDevice(deviceId, 1);
}

void SDL_Music::StopMusic()
{
    SDL_PauseAudioDevice(deviceId, 1);
    SDL_ClearQueuedAudio(deviceId);
    SDL_CloseAudioDevice(deviceId);
    deviceId = 0;
    IsItPlay = false;
    audio_len = wavLength;
    audio_pos = wavBuffer;
}
