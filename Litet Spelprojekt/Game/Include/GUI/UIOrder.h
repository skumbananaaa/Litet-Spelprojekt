#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class UIOrder : public IButtonListener
{
public:
	UIOrder();
	virtual ~UIOrder();

	void AddChoice(const std::string& name, void* userData) noexcept;

	virtual void OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept = 0;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

	void DisplayOrders(int32 x, int32 y, const std::vector<int32>& selectedMembers);

protected:

private:
	static void DisplayOrders(int32 x, int32 y, UIOrder* uiOrder);

	static Panel* m_pPanel;
	std::vector<std::pair<std::string, void*>> m_Choices;
	std::vector<int32> m_SelectedMembers;
};

