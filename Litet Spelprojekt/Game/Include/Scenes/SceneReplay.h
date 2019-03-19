#pragma once
#include "SceneGame.h"
#include "../GUI/UIReplay.h"

class SceneReplay : public SceneGame
{
public:
	SceneReplay(World* pWorld, float percentage = 0.0F);
	~SceneReplay();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* pRenderer) noexcept override;
	virtual void OnUpdate(float dtS) noexcept override;

private:
	UIReplay* m_UIReplay;
	float m_BeginPercentage;
};