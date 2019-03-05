#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class UIOrder
{
public:
	UIOrder();
	virtual ~UIOrder();

	void AddChoice(const std::string& name, void* userData) noexcept;

	virtual void OnOrderChosen(const std::string& name, void* userData) noexcept = 0;

	void DisplayOrders(int32 x, int32 y);

protected:

private:
	static void DisplayOrders(int32 x, int32 y, UIOrder* uiOrder);

	static Panel* m_pPanel;
	std::vector<std::pair<std::string, void*>> m_Choices;
};

