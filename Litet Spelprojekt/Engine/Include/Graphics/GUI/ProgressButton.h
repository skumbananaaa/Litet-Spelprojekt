#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/ProgressBar.h>

class ProgressButton;

class API IProgressListener
{
public:
	virtual void OnProgressAnimationEnd(ProgressButton* progressButton) = 0;
};

class API ProgressButton : public Button
{
public:
	ProgressButton(float x, float y, float width, float height, const std::string& text);
	virtual ~ProgressButton();

	void SetPercentage(float percentage);
	float GetPercentage() const noexcept;

	const glm::vec4& GetProgressColor() const noexcept;
	void SetProgressColor(const glm::vec4& color);

	void AddProgressListener(IProgressListener* listener);
	void RemoveProgressListener(IProgressListener* listener);

	void StartAnimation(float seconds) noexcept;

protected:
	virtual void OnUpdate(float dtS) override;
	virtual void OnRender(GUIContext* context) override;
	virtual void RenderRealTime(GUIContext* context, float x = 0, float y = 0) override;
	virtual void RenderProgress(GUIContext* context, float x = 0, float y = 0) noexcept;
	virtual void PrintName() const override;

private:
	float m_Percentage;
	glm::vec4 m_ProgressColor;
	float m_Timer;
	float m_TimeOfAnimation;
	std::vector<IProgressListener*> m_Listeners;
};