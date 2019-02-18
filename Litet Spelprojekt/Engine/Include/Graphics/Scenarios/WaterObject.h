#pragma once

#include <Graphics/GameObject.h>

constexpr float WATER_UPDATE_INTERVAL = 0.01f;
constexpr float WATER_ROUNDING_FACTOR = 100.0f;
constexpr float WATER_EVAPORATION_RATE = 1.0f / 1000.0f;
constexpr float WATER_AGING_DENOMINATOR = 10.0f;
constexpr float WATER_INV_TIME_FOR_WATER_TO_LEVEL = 30.0f;

class API WaterObject : public GameObject
{
public:
	WaterObject() noexcept;
	~WaterObject() override;

	void Update(float deltaTime) noexcept override;
};