#include <EnginePch.h>
#include <Audio/Music.h>
#include <vorbis/vorbisfile.h>

#define BUFFER_SIZE 32768

Music::Music(const char* const path) noexcept
{
	Create(path);
}

Music::~Music()
{
	alDeleteBuffers(1, &m_BufferId);
}

void Music::Create(const char* const path) noexcept
{
	//https://www.gamedev.net/articles/programming/general-and-gameplay-programming/introduction-to-ogg-vorbis-r2031/
	//Load OGG
	int endian = IsBigEndian() ? 1 : 0;
	FILE* file = fopen(path, "rb");

	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	ov_open(file, &oggFile, NULL, 0);

	pInfo = ov_info(&oggFile, -1);

	if (pInfo->channels == 1)
	{
		m_Format = AL_FORMAT_MONO16;
	}
	else
	{
		m_Format = AL_FORMAT_STEREO16;
	}

	m_SampleRate = pInfo->rate;

	long bytes;
	int bitStream;
	int8 array[BUFFER_SIZE];
	do {
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		m_Data.insert(m_Data.end(), array, array + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);

	//Create OpenAL Buffer
	alGenBuffers(1, &m_BufferId);
	alBufferData(m_BufferId, m_Format, &m_Data[0], static_cast<ALsizei>(m_Data.size()), m_SampleRate);
}

bool Music::IsBigEndian() noexcept
{
	int32 a = 1;
	return !((int8*)&a)[0];
}
