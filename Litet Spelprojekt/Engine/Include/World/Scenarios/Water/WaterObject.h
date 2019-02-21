#pragma once

#include <Graphics/GameObject.h>

constexpr float WATER_UPDATE_LEVEL_INTERVAL = 0.02f;
constexpr float WATER_ROUNDING_FACTOR = 50.0f;
constexpr float WATER_EVAPORATION_RATE = 1.0f / 1000.0f;
constexpr float WATER_AGING_DENOMINATOR = 10.0f;
constexpr float WATER_INV_TIME_FOR_WATER_TO_LEVEL = 30.0f;
constexpr float WATER_MAX_LEVEL = 2.0f;

class API WaterObject : public GameObject
{
public:
	WaterObject() noexcept;
	~WaterObject() override;

	void Update(float deltaTime) noexcept override;
};