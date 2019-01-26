#include <EnginePch.h>
#include <Audio/SoundEffect.h>

SoundEffect::SoundEffect(const char* const path) noexcept : m_BufferId(0)
{
	Create(path);
}

SoundEffect::~SoundEffect()
{
	alDeleteBuffers(1, &m_BufferId);
}

void SoundEffect::Create(const char* const path) noexcept
{
	int32 channel = 0;
	int32 sampleRate = 0;
	int32 bps = 0;
	int32 size = 0;

	char buffer[4];
	std::ifstream in(path, std::ios::binary);
	in.read(buffer, 4);

	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "This is not a valid WAV file!" << std::endl;
	}

	in.read(buffer, 4);
	in.read(buffer, 4);		//WAVE
	in.read(buffer, 4);		//fmt
	in.read(buffer, 4);		//16
	in.read(buffer, 2);		//1
	in.read(buffer, 2);
	channel = ConvertToInt(buffer, 2);
	in.read(buffer, 4);
	sampleRate = ConvertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = ConvertToInt(buffer, 2);
	in.read(buffer, 4);		//data
	in.read(buffer, 4);
	size = ConvertToInt(buffer, 4);

	char* data = new char[size];
	in.read(data, size);

	//Create AL Stuff
	uint32 format = 0;

	alGenBuffers(1, &m_BufferId);

	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}

	alBufferData(m_BufferId, format, data, size, sampleRate);
	delete[] data;
}

int32 SoundEffect::ConvertToInt(char* buffer, int length) noexcept
{
	int a = 0;
	if (!IsBigEndian())
	{
		for (int i = 0; i < length; i++)
		{
			((char*)&a)[i] = buffer[i];
		}
	}
	else
	{
		for (int i = 0; i < length; i++)
		{
			((char*)&a)[3 - i] = buffer[i];
		}
	}

	return a;
}

bool SoundEffect::IsBigEndian() noexcept
{
	int32 a = 1;
	return !((int8*)&a)[0];
}
