#pragma once
#include <Types.h>

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
	static bool HasCompletedScenarios();
	static void SetNumScenariosSelected(uint32 numScenearios);
	static void AddScenariosCompleted();

private:
	static float s_CrewHealth;
	static float s_BurningAmount;
	static float s_WaterLeakAmount;
	static uint32 s_NumScenariosSelected;
	static uint32 s_NumScenariosCompleted;
};