#include "Bang/AudioSource.h"

#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/ICloneable.h"
#include "Bang/AudioManager.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/EditorState.h"
#endif

AudioSource::AudioSource()
{
    alGenSources(1, &m_alSourceId);
}

AudioSource::~AudioSource()
{
    Stop();

    if (m_audioClip)
    {
        m_audioClip->OnAudioSourceDettached(this);
    }

    alDeleteSources(1, &m_alSourceId);
}

void AudioSource::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    AudioSource *as = Object::SCast<AudioSource>(clone);
    as->SetAudioClip( GetAudioClip() );
    as->SetVolume( GetVolume()  );
    as->SetPitch( GetPitch() );
    as->SetRange( GetRange() );
    as->SetLooping( IsLooping() );
    as->SetPlayOnStart( IsPlayOnStart() );
}

#ifdef BANG_EDITOR
void AudioSource::OnButtonClicked(const String &attrName)
{
    if (IsPlaying())
    {
        Stop();
    }
    else
    {
        Play();
    }
}
#endif

void AudioSource::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    AudioClip *audioClip = GetAudioClip();
    Path audioClipFilepath = audioClip ? audioClip->GetFilepath() : Path();
    Path newAudioClipFilepath = xmlInfo.GetFilepath("AudioClip");
    if (audioClipFilepath != newAudioClipFilepath)
    {
        SetAudioClip( AssetsManager::Load<AudioClip>(newAudioClipFilepath) );
    }
    SetVolume(xmlInfo.GetFloat("Volume"));
    SetPitch(xmlInfo.GetFloat("Pitch"));
    SetRange(xmlInfo.GetFloat("Range"));
    SetLooping(xmlInfo.GetBool("Looping"));
    SetPlayOnStart(xmlInfo.GetBool("PlayOnStart"));
}

void AudioSource::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    AudioClip *audioClip = GetAudioClip();
    Path audioClipFilepath = audioClip ? audioClip->GetFilepath() : Path();
    xmlInfo->SetFilepath("AudioClip", audioClipFilepath,
                         AudioClip::GetFileExtensionStatic());
    xmlInfo->SetFloat("Volume",     m_volume);
    xmlInfo->SetFloat("Pitch",      m_pitch);
    xmlInfo->SetFloat("Range",      m_range);
    xmlInfo->SetBool("Looping",     m_looping);
    xmlInfo->SetBool("PlayOnStart", m_playOnStart);

    AudioSource *noConstThis = const_cast<AudioSource*>(this);
    if (IsPlaying())
    {
        xmlInfo->SetButton("Stop", noConstThis, {});
        xmlInfo->SetButton("Play", noConstThis, {XMLProperty::Hidden});
    }
    else
    {

        xmlInfo->SetButton("Stop", noConstThis, {XMLProperty::Hidden});
        xmlInfo->SetButton("Play", noConstThis, {});
    }
}

void AudioSource::Play(float delay)
{
    ENSURE(m_audioClip);
    AudioManager::PlayAudioClip(m_audioClip, GetALSourceId(), delay);
}

void AudioSource::Pause()
{
    ENSURE(m_audioClip);
    alSourcePause(m_alSourceId);
}

void AudioSource::Stop()
{
    ENSURE(m_audioClip);
    alSourceStop(m_alSourceId);
}

AudioClip *AudioSource::GetAudioClip() const
{
    return m_audioClip;
}

void AudioSource::SetAudioClip(AudioClip *audioClip)
{
    if (m_audioClip)
    {
        m_audioClip->OnAudioSourceDettached(this);
    }

    SetAudioClipNoDettachAttach(audioClip);

    if (m_audioClip)
    {
        m_audioClip->OnAudioSourceAttached(this);
    }
}

void AudioSource::SetVolume(float volume)
{
    m_volume = volume;
    alSourcef(GetALSourceId(), AL_GAIN, m_volume);
}

void AudioSource::SetPitch(float pitch)
{
    m_pitch = Math::Max(pitch, 0.01f);
    alSourcef(GetALSourceId(), AL_PITCH, m_pitch);
}

void AudioSource::SetRange(float range)
{
    m_range = range;
    alSourcef(GetALSourceId(), AL_MAX_DISTANCE, m_range);
}

void AudioSource::SetLooping(bool looping)
{
    m_looping = looping;
    alSourcef(GetALSourceId(), AL_LOOPING, m_looping);
}

void AudioSource::SetPlayOnStart(bool playOnStart)
{
    m_playOnStart = playOnStart;
}

bool AudioSource::IsPlaying() const
{
    return GetState() == State::Playing;
}

bool AudioSource::IsPaused() const
{
    return GetState() == State::Paused;
}

bool AudioSource::IsStopped() const
{
    return GetState() == State::Stopped;
}

AudioSource::State AudioSource::GetState() const
{
    ALint state;
    alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &state);
    return static_cast<State>(state);
}

void AudioSource::OnStart()
{
    Component::OnStart();
    if (IsPlayOnStart())
    {
        #ifdef BANG_EDITOR
        if (EditorState::IsPlaying())
        #endif
        {
            Play();
        }
    }
}

float AudioSource::GetPlayProgress() const
{
	if (!m_audioClip) { return 0.0f; }

    float secondsOffset;
    alGetSourcef(m_alSourceId, AL_SEC_OFFSET, &secondsOffset);
    return secondsOffset / m_audioClip->GetLength();
}

float AudioSource::GetVolume() const
{
    return m_volume;
}

float AudioSource::GetPitch() const
{
    return m_pitch;
}

float AudioSource::GetRange() const
{
    return m_range;
}

bool AudioSource::IsLooping() const
{
    return m_looping;
}

bool AudioSource::IsPlayOnStart() const
{
    return m_playOnStart;
}

void AudioSource::OnUpdate()
{
    Component::OnUpdate();

    UpdateALProperties();
}

void AudioSource::OnDrawGizmos(bool depthed, bool overlay)
{
    Component::OnDrawGizmos(depthed, overlay);

    #ifdef BANG_EDITOR
    if (!depthed && !overlay)
    {
        Texture2D *tex = AssetsManager::Load<Texture2D>(
                    EPATH("Textures/AudioSourceIcon.btex2d") );
        Gizmos::SetPosition(transform->GetPosition());
        Gizmos::SetScale(Vector3::One * 0.1f);
        Gizmos::SetColor(Color::White);
        Gizmos::RenderIcon(tex);
    }

    if (depthed && gameObject->IsSelected())
    {
        Gizmos::SetColor(Color::Orange);
        Gizmos::SetPosition(transform->GetPosition());
        Gizmos::RenderSimpleSphere(transform->GetPosition(), m_range);
    }
    #endif
}

ALuint AudioSource::GetALSourceId() const
{
    return m_alSourceId;
}

void AudioSource::UpdateALProperties() const
{
    alSourcef(m_alSourceId,  AL_GAIN,         m_volume);
    alSourcef(m_alSourceId,  AL_PITCH,        m_pitch);
    alSourcei(m_alSourceId,  AL_LOOPING,      m_looping);
    Vector3 position = gameObject? transform->GetPosition() : Vector3::Zero;
    alSourcefv(m_alSourceId, AL_POSITION, position.Values());
    alSourcefv(m_alSourceId, AL_VELOCITY, Vector3::Zero.Values());

    alSourcef(m_alSourceId,  AL_MAX_DISTANCE, m_range);
    alSourcef(m_alSourceId,  AL_REFERENCE_DISTANCE, m_range * 0.5f);

    //Vector3 at = -transform->GetForward(), up = transform->GetUp();
    //ALfloat listenerOri[] = { at.x, at.y, at.z, up.x, up.y, up.z };
    //alSourcefv(m_alSourceId, AL_ORIENTATION, listenerOri);
}

void AudioSource::SetAudioClipNoDettachAttach(AudioClip *audioClip)
{
    m_audioClip = audioClip;
    alSourcei(m_alSourceId, AL_BUFFER,
              audioClip ? m_audioClip->GetALBufferId() : 0);
}
