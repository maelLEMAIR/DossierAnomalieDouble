#ifndef AUDIO_ENGINE_H_INCLUDED
#define AUDIO_ENGINE_H_INCLUDED

#include <windows.h>
#include <xaudio2.h>
#include <x3daudio.h>

#pragma comment(lib, "xaudio2.lib")

#include "define.h"

struct VoiceCallback : public IXAudio2VoiceCallback
{
    HANDLE hBufferEndEvent;
    bool finished   = false;
    bool isLoop     = false;
    bool paused     = false;

    IXAudio2SourceVoice*    pVoice  = nullptr;
    XAUDIO2_BUFFER          buffer  = {};

    VoiceCallback() {
        hBufferEndEvent = CreateEvent(nullptr, false, false, nullptr);
    }

    void OnBufferEnd(void*) override
    {
        if (isLoop && !paused && pVoice)
            pVoice->SubmitSourceBuffer(&buffer);
        else if (!isLoop)
            finished = true;
    }

    void OnVoiceProcessingPassStart(UINT32) override {}
    void OnVoiceProcessingPassEnd() override {}
    void OnStreamEnd() override {}
    void OnBufferStart(void*) override {}
    void OnLoopEnd(void*) override {}
    void OnVoiceError(void*, HRESULT) override {}
};

struct Sound
{
    WAVEFORMATEX format;
    XAUDIO2_BUFFER buffer;
    BYTE* data;
};

struct ActiveVoice
{
    IXAudio2SourceVoice*    pSourceVoice    = nullptr;
    VoiceCallback*          callback        = nullptr;

    void Pause()
    {
        if (!callback || !pSourceVoice) return;
        callback->paused = true;
        pSourceVoice->Stop(0);
    }

    void Resume()
    {
        if (!callback || !pSourceVoice) return;
        callback->paused = false;
        XAUDIO2_VOICE_STATE state;
        pSourceVoice->GetState(&state);
        if (state.BuffersQueued == 0)
            pSourceVoice->SubmitSourceBuffer(&callback->buffer);
        pSourceVoice->Start(0);
    }
};

struct SpatialVoice
{
    IXAudio2SourceVoice*    pSourceVoice    = nullptr;
    IXAudio2SubmixVoice*    pSubmixVoice    = nullptr;
    X3DAUDIO_EMITTER        emitter         = {};
    float                   dspMatrix[8]    = {};
};

class AudioEngine
{
public:

    AudioEngine() { s_pInstance = this; }
    
    bool Init();
    void Shutdown();

    static Sound* LoadWav(const WString& path);
    static ActiveVoice* PlaySoundW(Sound* sound, float volume = 1.0f, bool loop = false);
    static void PlaySoundOnce(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

    static SpatialVoice* Play3D(Sound* sound, float x, float y, float z, float volume = 1.0f, bool loop = false);

    static void SetPosListener(X3DAUDIO_VECTOR _pos, X3DAUDIO_VECTOR _front, X3DAUDIO_VECTOR _top);
    
    static void Update3D(SpatialVoice* sv, float ex, float ey, float ez);
    
    void CleanupFinishedVoices();

    void SetMasterVolume(float volume);

    static AudioEngine* s_pInstance;
private:
    bool Init3DAudio();

    IXAudio2*               m_pEngine       = nullptr;
    IXAudio2MasteringVoice* m_pMasterVoice  = nullptr;

    X3DAUDIO_HANDLE         m_x3dInstance   = {};
    DWORD                   m_nChannelMask  = 0;
    UINT32                  m_nOutputChannels = 0;

    X3DAUDIO_LISTENER       m_listener      = {};

    Vector<ActiveVoice*>    m_vActiveVoices;
    Vector<SpatialVoice*>   m_vSpatialVoices;

    bool LoadWavFile(const WString& filename, Sound& sound);
};

#endif

