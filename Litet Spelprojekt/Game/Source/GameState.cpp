#include "../Include/GameState.h"

float GameState::s_BurningAmount = 0.0f;
float GameState::s_CrewHealth = 1.0f;
float GameState::s_WaterLeakAmount = 0.0f;
uint32 GameState::s_NumScenariosSelected = 1; //Set to one, otherwise does not work
uint32 GameState::s_NumScenariosCompleted = 0;

void GameState::Reset()
{
	s_BurningAmount = 0.0f;
	s_CrewHealth = 1.0f;
	s_WaterLeakAmount = 0.0f;
	s_NumScenariosSelected = 1; //Set to one, otherwise does not work
	s_NumScenariosCompleted = 0;
}

void GameState::SetWaterLeakAmount(float waterAmount)
{
	s_WaterLeakAmount = waterAmount;
}

void GameState::SetBurningAmount(float burningAmount)
{
	s_BurningAmount = burningAmount;
}

void GameState::SetCrewHealth(float health)
{
	s_CrewHealth = health;
}

float GameState::GetWaterLeakAmount()
{
	return s_WaterLeakAmount;
}

float GameState::GetBurningAmount()
{
	return s_BurningAmount;
}

float GameState::GetCrewHealth()
{
	return s_CrewHealth;
}

bool GameState::HasCompletedScenarios()
{
	return s_NumScenariosCompleted >= s_NumScenariosSelected;
}

void GameState::SetNumScenariosSelected(uint32 numScenearios)
{
	s_NumScenariosSelected = numScenearios;
}

void GameState::AddScenariosCompleted()
{
	s_NumScenariosCompleted++;
}
