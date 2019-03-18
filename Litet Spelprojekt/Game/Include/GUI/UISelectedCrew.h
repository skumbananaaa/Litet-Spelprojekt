#pragma once
#include <Graphics/GUI/ProgressButton.h>

class UISelectedCrew : public ProgressButton
{
public:
	UISelectedCrew(const std::string& name);
	virtual ~UISelectedCrew();

	void UpdatePosition(const glm::vec3& position) noexcept;

protected:
	virtual void PrintName() const override;
};