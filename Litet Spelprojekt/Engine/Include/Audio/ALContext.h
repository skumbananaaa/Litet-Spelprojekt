#pragma once
#include "IAudioContext.h"

enum DistanceModel : uint8
{
	INVERSE_DISTANCE = 0xD001,
	INVERSE_DISTANCE_CLAMPED = 0xD001,
	LINEAR_DISTANCE = 0xD003,
	LINEAR_DISTANCE_CLAMPED = 0xD004,
	EXPONENT_DISTANCE = 0xD005,
	EXPONENT_DISTANCE_CLAMPED = 0xD006,
};

class ALContext : public IAudioContext
{
public:
	ALContext(ALContext&& other) = delete;
	ALContext(const ALContext& other) = delete;
	ALContext& operator=(ALContext&& other) = delete;
	ALContext& operator=(const ALContext& other) = delete;

	ALContext();
	~ALContext();

	void SetDistanceModel(DistanceModel distanceModel) const noexcept;

private:
	ALCdevice* m_pDevice;
	ALCcontext* m_pContext;

public:
	static bool HasErrors();

private:
	static void ListAudioDevices();
};