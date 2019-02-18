#include <EnginePch.h>
#include <World/VisualAudioSource.h>

VisualAudioSource * VisualAudioSource::CreateObject(int32 sound, int32 mesh, int32 material, float volume, float attenuation)
{
	VisualAudioSource * tmp = new VisualAudioSource(sound, mesh, material);
	tmp->m_pAudio->SetVolume(volume);
	tmp->m_pAudio->SetRollOffFactor(attenuation);
	return tmp;
}

VisualAudioSource * VisualAudioSource::CreateResource(int32 sound)
{
	VisualAudioSource * tmp = new VisualAudioSource();
	tmp->m_pAudio = AudioSource::CreateSoundSource(sound);
	return tmp;
}

VisualAudioSource::~VisualAudioSource()
{
	delete m_pAudio;
}

VisualAudioSource::VisualAudioSource(int32 audioSrc, int32 mesh, int32 material): GameObject()
{
	GameObject::SetMesh(mesh);
	GameObject::SetMaterial(material);
	GameObject::SetPosition(glm::vec3(0.0f));
	m_pAudio = AudioSource::CreateSoundSource(audioSrc);
}

VisualAudioSource::VisualAudioSource() : GameObject()
{
}