#pragma once
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ProgressBar.h>

class UISelectedCrew : public TextView
{
public:
	UISelectedCrew(const std::string& name);
	virtual ~UISelectedCrew();

	void UpdatePosition(const glm::vec3& position) noexcept;
	void SetPercentage(float percentage);

protected:
	virtual void OnRemoved(GUIObject* parent) override;
	virtual void PrintName() const override;

private:
	ProgressBar* m_pProgressBar;
};