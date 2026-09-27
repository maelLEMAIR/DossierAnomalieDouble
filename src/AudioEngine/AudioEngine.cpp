#ifndef AUDIO_ENGINE_CPP_INCLUDED
#define AUDIO_ENGINE_CPP_INCLUDED

#include "AudioEngine.h"

AudioEngine* AudioEngine::s_pInstance = nullptr;

bool AudioEngine::Init()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != S_FALSE && hr != RPC_E_CHANGED_MODE)
        return false;

    hr = XAudio2Create(&m_pEngine, 0);
    if (FAILED(hr)) return false;

    hr = m_pEngine->CreateMasteringVoice(&m_pMasterVoice);
    if (FAILED(hr)) return false;

    if (!Init3DAudio())
        return false;

    m_listener.OrientFront = { 0.f, 0.f, 1.f };
    m_listener.OrientTop   = { 0.f, 1.f, 0.f };
    m_listener.Position    = { 0.f, 0.f, 0.f };
    m_listener.Velocity    = { 0.f, 0.f, 0.f };

    return true;
}

void AudioEngine::Shutdown()
{
    for (auto v : m_vActiveVoices)
        v->pSourceVoice->DestroyVoice();

    m_vActiveVoices.clear();

    if (m_pMasterVoice)
    {
        m_pMasterVoice->DestroyVoice();
        m_pMasterVoice = nullptr;
    }

    if (m_pEngine)
    {
        m_pEngine->Release();
        m_pEngine = nullptr;
    }

    CoUninitialize();
}

Sound* AudioEngine::LoadWav(const WString& path)
{
    Sound* sound = new Sound();

    if (s_pInstance->LoadWavFile(path, *sound) == false)
    {
        delete sound;
        return nullptr;
    }

    return sound;
}

ActiveVoice* AudioEngine::PlaySoundW(Sound* sound, float volume, bool loop)
{
    VoiceCallback* cb       = new VoiceCallback();
    cb->isLoop              = loop;
    cb->buffer              = sound->buffer;
    cb->buffer.LoopCount    = 0;
    cb->buffer.Flags        = XAUDIO2_END_OF_STREAM;

    IXAudio2SourceVoice* voice;
    HRESULT hr = s_pInstance->m_pEngine->CreateSourceVoice(
        &voice, &sound->format,
        0, XAUDIO2_DEFAULT_FREQ_RATIO, cb);
    if (FAILED(hr)) { delete cb; return nullptr; }

    cb->pVoice = voice;
    voice->SetVolume(volume);
    voice->SubmitSourceBuffer(&cb->buffer);
    voice->Start(0);

    ActiveVoice* actVoice   = new ActiveVoice();
    actVoice->pSourceVoice  = voice;
    actVoice->callback      = cb;
    s_pInstance->m_vActiveVoices.push_back(actVoice);

    return actVoice;
}

void AudioEngine::PlaySoundOnce(Sound* sound, float volume, float pitch)
{
    VoiceCallback* cb = new VoiceCallback();
    cb->isLoop = false;
    cb->buffer = sound->buffer;
    cb->buffer.Flags = XAUDIO2_END_OF_STREAM;

    IXAudio2SourceVoice* voice;
    HRESULT hr = s_pInstance->m_pEngine->CreateSourceVoice(
        &voice, &sound->format,
        0, XAUDIO2_DEFAULT_FREQ_RATIO, cb);
    if (FAILED(hr)) { delete cb; return; }

    cb->pVoice = voice;
    voice->SetVolume(volume);
    voice->SetFrequencyRatio(pitch);
    voice->SubmitSourceBuffer(&cb->buffer);
    voice->Start(0);

    ActiveVoice* av  = new ActiveVoice();
    av->pSourceVoice = voice;
    av->callback     = cb;
    s_pInstance->m_vActiveVoices.push_back(av);
}

void AudioEngine::CleanupFinishedVoices()
{
    auto it = m_vActiveVoices.begin();
    while (it != m_vActiveVoices.end())
    {
        if ((*it)->callback->finished)
        {
            (*it)->pSourceVoice->DestroyVoice();
            delete (*it)->callback;
            it = m_vActiveVoices.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool AudioEngine::Init3DAudio()
{
    DWORD channelMask;
    m_pMasterVoice->GetChannelMask(&channelMask);
    m_nChannelMask = channelMask;

    XAUDIO2_VOICE_DETAILS details;
    m_pMasterVoice->GetVoiceDetails(&details);
    m_nOutputChannels = details.InputChannels;

    std::cout << "ChannelMask: 0x" << std::hex << m_nChannelMask << "\n";
    std::cout << "OutputChannels: " << std::dec << m_nOutputChannels << "\n";
    
    HRESULT hr = X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dInstance);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize X3DAudio\n";
        return false;
    }

    return true;
}

SpatialVoice* AudioEngine::Play3D(Sound* sound, float x, float y, float z, float volume, bool loop)
{
    AudioEngine* eng = s_pInstance;

    IXAudio2SourceVoice* voice = nullptr;
    HRESULT hr = eng->m_pEngine->CreateSourceVoice(
        &voice, &sound->format,
        0, XAUDIO2_DEFAULT_FREQ_RATIO,
        nullptr, nullptr);
    if (FAILED(hr)) return nullptr;

    voice->SetVolume(volume);
    voice->SubmitSourceBuffer(&sound->buffer);
    voice->Start(0);

    SpatialVoice* sv = new SpatialVoice();
    sv->pSourceVoice = voice;
    sv->pSubmixVoice = nullptr;

    sv->emitter.ChannelCount         = 1;
    sv->emitter.CurveDistanceScaler  = 10.f;
    sv->emitter.OrientFront          = { 0.f, 0.f, 1.f };
    sv->emitter.OrientTop            = { 0.f, -1.f, 0.f };
    sv->emitter.Position             = { x, y, z };
    sv->emitter.Velocity             = { 0.f, 0.f, 0.f };

    X3DAUDIO_DSP_SETTINGS dsp = {};
    dsp.SrcChannelCount     = 1;
    dsp.DstChannelCount     = eng->m_nOutputChannels;
    dsp.pMatrixCoefficients = sv->dspMatrix;

    X3DAudioCalculate(
        eng->m_x3dInstance,
        &eng->m_listener,
        &sv->emitter,
        X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
        &dsp);
    
    // Juste après X3DAudioCalculate dans Play3D
    std::cout << "DSP matrix: L=" << sv->dspMatrix[0] << " R=" << sv->dspMatrix[1] << "\n";
    voice->SetOutputMatrix(nullptr, 1, eng->m_nOutputChannels, sv->dspMatrix);

    eng->m_vSpatialVoices.push_back(sv);
    return sv;
}

void AudioEngine::SetPosListener(X3DAUDIO_VECTOR _pos, X3DAUDIO_VECTOR _front, X3DAUDIO_VECTOR _top)
{
    AudioEngine* eng = s_pInstance;
    eng->m_listener.Position = _pos;
    eng->m_listener.OrientFront = _front;
    eng->m_listener.OrientTop = _top;
}

void AudioEngine::Update3D(SpatialVoice* sv, float ex, float ey, float ez)
{
    AudioEngine* eng = s_pInstance;

    sv->emitter.Position = { ex, ey, ez };

    X3DAUDIO_DSP_SETTINGS dsp = {};
    dsp.SrcChannelCount     = 1;
    dsp.DstChannelCount     = eng->m_nOutputChannels;
    dsp.pMatrixCoefficients = sv->dspMatrix;

    X3DAudioCalculate(
        eng->m_x3dInstance,
        &eng->m_listener,
        &sv->emitter,
        X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT,
        &dsp);
    
    sv->pSourceVoice->SetOutputMatrix(nullptr, 1, eng->m_nOutputChannels, sv->dspMatrix);
    sv->pSourceVoice->SetFrequencyRatio(dsp.DopplerFactor);
}

bool AudioEngine::LoadWavFile(const WString& filename, Sound& sound)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
        return false;

    char riff[4];
    uint32_t size;
    char wave[4];

    file.read(riff, 4);
    file.read((char*)&size, 4);
    file.read(wave, 4);

    if (memcmp(riff, "RIFF", 4) != 0 || memcmp(wave, "WAVE", 4) != 0)
        return false;

    char chunkId[4];
    uint32_t chunkSize;

    while (file.read(chunkId, 4))
    {
        file.read((char*)&chunkSize, 4);

        if (memcmp(chunkId, "fmt ", 4) == 0)
        {
            WORD  formatTag, channels;
            DWORD sampleRate, byteRate;
            WORD  blockAlign, bitsPerSample;

            file.read((char*)&formatTag,     2);
            file.read((char*)&channels,      2);
            file.read((char*)&sampleRate,    4);
            file.read((char*)&byteRate,      4);
            file.read((char*)&blockAlign,    2);
            file.read((char*)&bitsPerSample, 2);

            sound.format.wFormatTag      = formatTag;
            sound.format.nChannels       = channels;
            sound.format.nSamplesPerSec  = sampleRate;
            sound.format.nAvgBytesPerSec = byteRate;
            sound.format.nBlockAlign     = blockAlign;
            sound.format.wBitsPerSample  = bitsPerSample;
            sound.format.cbSize          = 0;

            std::cout << "Channels: " << sound.format.nChannels << "\n";
            std::cout << "SampleRate: " << sound.format.nSamplesPerSec << "\n";
            
            if (chunkSize > 16)
                file.seekg(chunkSize - 16, std::ios::cur);
        }
        else if (memcmp(chunkId, "data", 4) == 0)
        {
            BYTE* data = new BYTE[chunkSize];
            file.read((char*)data, chunkSize);

            sound.data = data;

            sound.buffer = {};
            sound.buffer.AudioBytes = chunkSize;
            sound.buffer.pAudioData = data;
            sound.buffer.Flags      = XAUDIO2_END_OF_STREAM;

            std::cout << "Channels: " << sound.format.nChannels << "\n";
            std::cout << "SampleRate: " << sound.format.nSamplesPerSec << "\n";
            
            return true;
        }
        else
        {
            file.seekg(chunkSize, std::ios::cur);
        }
    }

    return false;
}

void AudioEngine::SetMasterVolume(float volume)
{
    m_pMasterVoice->SetVolume(volume);
}

#endif