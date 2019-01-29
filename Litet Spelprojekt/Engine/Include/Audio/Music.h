#pragma once
#include <EnginePch.h>

class API Music
{
	friend class AudioSource;

public:
	Music(Music&& other) = delete;
	Music(const Music& other) = delete;
	Music& operator=(Music&& other) = delete;
	Music& operator=(const Music& other) = delete;

	explicit Music(const char* const path) noexcept;
	~Music();

private:
	void Create(const char* const path) noexcept;

private:
	uint32 m_BufferId;
	std::vector<uint8> m_Data;
	int m_Format;
	int m_SampleRate;

private:
	static bool IsBigEndian() noexcept;
};