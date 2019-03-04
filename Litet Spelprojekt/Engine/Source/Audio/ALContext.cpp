#include <EnginePch.h>
#include <Audio/ALContext.h>

ALContext::ALContext() :
	m_pDevice(nullptr),
	m_pContext(nullptr)
{
	ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_TRUE)
	{
		ListAudioDevices();
		m_pDevice = alcOpenDevice(NULL);
		std::cout << "Default Audio Device selected" << std::endl;
	}
	else
	{
		m_pDevice = alcOpenDevice(NULL);
	}

	if (m_pDevice == nullptr)
	{
		std::cout << "Could not create OpenAL Device Handle" << std::endl;
	}
	else
	{
		m_pContext = alcCreateContext(m_pDevice, NULL);
		if (alcMakeContextCurrent(m_pContext) != AL_TRUE)
		{
			std::cout << "Could not make OpenAL Context Current" << std::endl;
		}
		else
		{
			if (ALContext::HasErrors())
			{
				std::cout << "OpenAL could not be initialized" << std::endl;
			}
			else
			{
				std::cout << "OpenAL initialized" << std::endl;
			}
		}
	}
}

ALContext::~ALContext()
{
	if (m_pContext != nullptr)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_pContext);
		m_pContext = nullptr;
	}

	if (m_pDevice != nullptr)
	{
		alcCloseDevice(m_pDevice);
		m_pDevice = nullptr;
	}
}

void ALContext::SetDistanceModel(DistanceModel distanceModel) const noexcept
{
	alDistanceModel(distanceModel);
}

bool ALContext::HasErrors()
{
	ALCenum error;
	bool result = false;

	while ((error = alGetError()) != AL_NO_ERROR)
	{
		result = true;
		std::cout << "OpenAL Error: " << std::to_string(error) << std::endl;
	}

	return result;
}

void ALContext::ListAudioDevices()
{
	const ALCchar* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	const ALCchar* device = devices;
	const ALCchar* next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "OpenAL Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}
