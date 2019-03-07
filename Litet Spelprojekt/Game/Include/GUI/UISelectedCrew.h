#pragma once
#include <Graphics/GUI/TextView.h>

class UISelectedCrew : public TextView
{
public:
	UISelectedCrew(const std::string& name);
	virtual ~UISelectedCrew();

	void UpdatePosition(const glm::vec3& position) noexcept;

protected:
	virtual void PrintName() const override;
};