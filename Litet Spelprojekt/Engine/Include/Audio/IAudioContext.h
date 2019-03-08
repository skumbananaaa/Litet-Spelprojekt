#pragma once
#include <EnginePch.h>

enum DistanceModel : uint32
{
	INVERSE_DISTANCE = 0xD001,
	INVERSE_DISTANCE_CLAMPED = 0xD001,
	LINEAR_DISTANCE = 0xD003,
	LINEAR_DISTANCE_CLAMPED = 0xD004,
	EXPONENT_DISTANCE = 0xD005,
	EXPONENT_DISTANCE_CLAMPED = 0xD006,
};

class API IAudioContext
{
public:
	IAudioContext() {}
	virtual ~IAudioContext() {}
	virtual void SetDistanceModel(DistanceModel distanceModel) const noexcept = 0;

public:
	static IAudioContext* CreateContext();
};