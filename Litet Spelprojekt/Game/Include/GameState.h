#pragma once

class GameState
{
public:
	GameState() = delete;
	~GameState() = delete;
	GameState(GameState&&) = delete;
	GameState(const GameState&) = delete;
	GameState& operator=(GameState&&) = delete;
	GameState& operator=(const GameState&) = delete;

public:
	static void Reset();
	static void SetWaterLeakAmount(float waterAmount);
	static void SetBurningAmount(float waterAmount);
	static void SetCrewHealth(float health);
	static float GetWaterLeakAmount();
	static float GetBurningAmount();
	static float GetCrewHealth();

private:
	static float s_CrewHealth;
	static float s_BurningAmount;
	static float s_WaterLeakAmount;
};