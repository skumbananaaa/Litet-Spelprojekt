#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>

class ProgressBar;

class API IProgressListener
{
public:
	virtual ~IProgressListener() {};
	virtual void OnProgressChange(ProgressBar* pProgressBar, float percentage) = 0;

protected:
	IProgressListener() {};
};

class API ProgressBar : public GUIObject
{
public:
	ProgressBar(float x, float y, float width, float height);
	virtual ~ProgressBar();

	void SetPercentage(float percentage);
	float GetPercentage() const noexcept;

	const glm::vec4& GetProgressColor() const noexcept;
	void SetProgressColor(const glm::vec4& color);

	void BeginProgress() noexcept;
	void EndProgress() noexcept;

	void AddProgressListener(IProgressListener* listener);
	void RemoveProgressListener(IProgressListener* listener);

protected:
	virtual void OnRender(GUIContext* context) override;
	virtual void RenderRealTime(GUIContext* context, float x = 0, float y = 0) override;
	virtual void RenderProgress(GUIContext* context, float x = 0, float y = 0) noexcept;
	virtual void PrintName() const override;

private:
	float m_Percentage;
	glm::vec4 m_ProgressColor;
	std::vector<IProgressListener*> m_ProgressListeners;
};