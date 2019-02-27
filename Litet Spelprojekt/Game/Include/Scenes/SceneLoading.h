#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ProgressBar.h>

class SceneLoading : public SceneInternal
{
public:
	SceneLoading();
	virtual ~SceneLoading();

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

	virtual void OnResourceLoading(const std::string& file, float percentage) override;
	virtual void OnResourcesLoaded() override;

private:
	TextView* m_pTextViewFile;
	ProgressBar* m_pLoadingBar;
};
