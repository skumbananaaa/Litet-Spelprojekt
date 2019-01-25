#pragma once

#include <EnginePch.h>

class API SoundEffect
{
	friend class AudioSource;

public:
	SoundEffect(SoundEffect&& other) = delete;
	SoundEffect(const SoundEffect& other) = delete;
	SoundEffect& operator=(SoundEffect&& other) = delete;
	SoundEffect& operator=(const SoundEffect& other) = delete;

	explicit SoundEffect(const char* const path) noexcept;
	~SoundEffect();

private:
	void Create(const char* const path) noexcept;

private:
	uint32 m_BufferId;

private:
	static int32 ConvertToInt(char* buffer, int length) noexcept;
	static bool IsBigEndian() noexcept;
};