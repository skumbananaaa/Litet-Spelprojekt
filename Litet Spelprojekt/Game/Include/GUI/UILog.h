#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ListScrollable.h>
#include <World/Logger.h>

class UILog : public Panel, public ILogListener
{
public:
	UILog(float x, float y, float width, float height);
	virtual ~UILog();

	virtual void OnLogged(const std::string& text) noexcept override;

protected:
	virtual void OnUpdate(float dtS) override;
	virtual void OnRender(GUIContext* context) override;

	virtual void OnHovered(const glm::vec2& position) override;
	virtual void OnNotHovered(const glm::vec2& position) override;

	virtual void PrintName() const override;

private:
	ListScrollable* m_ListScrollableLog;
	TextView* m_pTextViewLog;
	bool m_Extend;
	float m_Percentage;
	float m_DefaultX;
};
