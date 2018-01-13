#include "Bang/AudioManager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <cstring>

#include "Bang/Path.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Vector3.h"
#include "Bang/Resources.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/ALAudioSource.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioPlayerRunnable.h"

USING_NAMESPACE_BANG

AudioManager::AudioManager()
{
}

void AudioManager::Init()
{
    InitAL();
    m_threadPool.SetMaxThreadCount(256);
}


AudioManager::~AudioManager()
{
    for (AudioPlayerRunnable *audioPlayer : m_currentAudioPlayers)
    {
        ALuint sourceId = audioPlayer->GetALAudioSource()->GetALSourceId();
        alDeleteSources(1, &sourceId);
    }
    StopAllSounds();

    alcDestroyContext(m_alContext);
    alcCloseDevice(m_alDevice);
}


bool AudioManager::InitAL()
{
    bool extIsPresent = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (!extIsPresent)
    {
        Debug_Error("Enumeration extension not available.");
        return false;
    }

    String defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    m_alDevice = alcOpenDevice(defaultDeviceName.ToCString());
    if(!m_alDevice)
    {
        Debug_Error("Could not start OpenAL Device '" << defaultDeviceName << "'");
        return false;
    }

    m_alContext = alcCreateContext(m_alDevice, NULL);
    if(!m_alContext)
    {
        Debug_Error("Could not start OpenAL Context");
        return false;
    }

    bool currentContextResult = alcMakeContextCurrent(m_alContext);
    if (!currentContextResult)
    {
        Debug_Error("Could not set OpenAL Context");
        return false;
    }

    return true;
}

String AudioManager::GetALErrorEnumString(ALenum errorEnum)
{
    switch (errorEnum)
    {
        case AL_NO_ERROR:          return "AL_NO_ERROR";
        case AL_INVALID_NAME:      return "AL_INVALID_NAME";
        case AL_INVALID_ENUM:      return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE:     return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY:     return "AL_OUT_OF_MEMORY";
    }
    ASSERT(false);
    return "";
}

String AudioManager::GetALCErrorEnumString(ALCenum errorEnum)
{
    switch (errorEnum)
    {
        case ALC_NO_ERROR:        return "ALC_NO_ERROR";
        case ALC_INVALID_DEVICE:  return "ALC_INVALID_DEVICE";
        case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
        case ALC_INVALID_ENUM:    return "ALC_INVALID_ENUM";
        case ALC_INVALID_VALUE:   return "ALC_INVALID_VALUE";
        case ALC_OUT_OF_MEMORY:   return "ALC_OUT_OF_MEMORY";
    }
    ASSERT(false);
    return "";
}

void AudioManager::Play(AudioClip *audioClip,
                        ALAudioSource *aas,
                        float delay)
{
    ENSURE(audioClip);
    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip,
                                                          aas, delay);
    AudioManager *am = AudioManager::GetInstance();
    bool started = am->m_threadPool.TryStart(player);
    if (started)
    {
        MutexLocker m(&am->m_mutex_currentAudios);
        am->m_currentAudioPlayers.Add(player);
    }
}

void AudioManager::Play(AudioClip *audioClip,
                        const AudioParams &params,
                        float delay)
{
    ENSURE(audioClip);
    ALAudioSource *aas = new ALAudioSource();
    aas->SetALBufferId(audioClip->GetALBufferId());
    aas->SetParams(params);
    aas->m_autoDelete = true;
    AudioManager::Play(audioClip, aas, delay);
}

void AudioManager::Play(const Path &audioClipFilepath,
                        const AudioParams &params,
                        float delay)
{
    RH<AudioClip> audioClip = Resources::Load<AudioClip>(audioClipFilepath);
    AudioManager::Play(audioClip.Get(), params, delay);
}

void AudioManager::PauseAllSounds()
{
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *audioPlayer : am->m_currentAudioPlayers)
    {
        audioPlayer->Pause();
    }
}

void AudioManager::ResumeAllSounds()
{
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *audioPlayer : am->m_currentAudioPlayers)
    {
        audioPlayer->Resume();
    }
}

void AudioManager::StopAllSounds()
{
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *audioPlayer : am->m_currentAudioPlayers)
    {
        audioPlayer->Stop();
    }

    MutexLocker m(&am->m_mutex_currentAudios);
    am->m_currentAudioPlayers.Clear();
}

void AudioManager::OnAudioFinishedPlaying(AudioPlayerRunnable *audioPlayer)
{
    MutexLocker m(&m_mutex_currentAudios);
    m_currentAudioPlayers.Remove(audioPlayer);
}

void AudioManager::DettachSourcesFromAudioClip(AudioClip *ac)
{
    // Dettach all audioSources using this AudioClip.
    // Otherwise OpenAL throws error.
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *ap : am->m_currentAudioPlayers)
    {
        if (ap->GetAudioClip() == ac)
        {
            ap->Stop();
            ap->GetALAudioSource()->SetALBufferId(0);
        }
    }
}

void AudioManager::ClearALErrors()
{
    alGetError();
    ALCdevice *device = AudioManager::GetInstance()->m_alDevice;
    if (device) { alcGetError(device); }
}

bool AudioManager::CheckALError()
{
    bool someError = false;
    {
        ALenum error = alGetError();
        bool hasError = (error != AL_NO_ERROR);
        if (hasError)
        {
            const char *errorStr = alGetString(error);
            Debug_Error("OpenAL al error(" <<
                        AudioManager::GetALErrorEnumString(error) <<
                        "): " <<
                        errorStr);
            someError = true;
        }
    }

    ALCdevice *device = AudioManager::GetInstance()->m_alDevice;
    if (device)
    {
        ALenum error = alcGetError(device);
        bool hasError = (error != AL_NO_ERROR);
        if (hasError)
        {
            const char *errorStr = alcGetString(device, error);
            Debug_Error("OpenAL alc error(" <<
                        AudioManager::GetALCErrorEnumString(error) <<
                        "): " <<
                        errorStr);
            someError = true;
        }
        alcGetError(device);
    }

    return someError;
}

List<String> AudioManager::GetAudioDevicesList()
{
    List<String> audioDevicesList;

    const ALCchar *devices     = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    const ALCchar *currentChar = devices;
    const ALCchar *nextChar    = currentChar + 1;

    while (currentChar && *currentChar != '\0' &&
           nextChar    && *nextChar    != '\0')
    {
        String currentDevice = String(currentChar);
        audioDevicesList.PushBack(currentDevice);

        size_t len  = strlen(currentChar);
        currentChar += (len + 1);
        nextChar    += (len + 2);
    }

    return audioDevicesList;
}

AudioManager *AudioManager::GetInstance()
{
    return Application::GetInstance()->GetAudioManager();
}
