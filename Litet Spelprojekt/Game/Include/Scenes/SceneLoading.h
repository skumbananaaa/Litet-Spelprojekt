#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ProgressBar.h>

class SceneLoading : public SceneInternal
{
	friend class Game;

public:
	virtual ~SceneLoading();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

	virtual void OnResourceLoading(const std::string& file, float percentage) override;
	virtual void OnResourcesLoaded() override;

protected:
	SceneLoading();

private:
	TextView* m_pTextViewFile;
	ProgressBar* m_pLoadingBar;
};
