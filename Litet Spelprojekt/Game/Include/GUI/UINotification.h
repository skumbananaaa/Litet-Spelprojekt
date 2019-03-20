#pragma once
#include <Graphics/GUI/Panel.h>

class UINotification : public IExternalUIRenderer
{
public:
	UINotification(float x, float y, float width, float height, float time);
	virtual ~UINotification();

	void CreateNotification(const std::string& text);

	void Update(float dtS) noexcept;

	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) override;

	void DeleteAll() noexcept;

	void SetVisible(bool visible) noexcept;

private:
	float m_Time;
	float m_PixelsToMove;
	glm::vec4 m_Bounds;
	std::vector<std::pair<Panel*, float>> m_Notifications;
	std::vector<Panel*> m_NotificationsToDelete;
};
