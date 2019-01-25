#pragma once
#include "IAudioContext.h"

class ALContext : public IAudioContext
{
public:
	ALContext(ALContext&& other) = delete;
	ALContext(const ALContext& other) = delete;
	ALContext& operator=(ALContext&& other) = delete;
	ALContext& operator=(const ALContext& other) = delete;

	ALContext();
	~ALContext();

private:
	ALCdevice* m_pDevice;
	ALCcontext* m_pContext;

public:
	static bool HasErrors();

private:
	static void ListAudioDevices();
};