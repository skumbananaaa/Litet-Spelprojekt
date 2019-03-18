#pragma once
#include "SceneGame.h"

class SceneReplay : public SceneGame
{
public:
	SceneReplay(World* pWorld);
	~SceneReplay();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;

};