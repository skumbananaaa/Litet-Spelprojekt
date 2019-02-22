#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Button.h>

class PanelExpandable;

class API IExpandableListener
{
public:
	virtual void OnExpanding(PanelExpandable* panel, float percentage) = 0;
	virtual void OnCollapsing(PanelExpandable* panel, float percentage) = 0;
};

class API PanelExpandable : public Button
{
public:
	PanelExpandable(float x, float y, float width, float height, float clientHeight, const std::string& text);
	virtual ~PanelExpandable();

	void SetExpanded(bool expanded) noexcept;
	bool IsExpanded() const noexcept;

	float GetClientWidth() const noexcept;
	float GetClientHeight() const noexcept;

	void AddExpandableListener(IExpandableListener* listener);
	void RemoveExpandableListener(IExpandableListener* listener);

	const glm::vec4& GetClientAreaColor() const noexcept;
	void SetClientAreaColor(const glm::vec4& color);

	virtual void SetSelected(bool selected) override;

	float GetPercentage() const noexcept;
	float GetYForClientArea() const noexcept;
	virtual float GetYInWorld(const GUIObject* child = nullptr) const noexcept;

	void SetClientSize(float height) noexcept;

	void RenderChildrensFrameBuffers(GUIContext* context) override;

	virtual void RenderRealTimePre(GUIContext* context, float x = 0, float y = 0) override;
	virtual void RenderRealTime(GUIContext* context, float x = 0, float y = 0) override;
	virtual void RenderRealTimePost(GUIContext* context) override;

	virtual bool ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept override;
	virtual bool OwnsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept override;

protected:
	virtual void OnUpdate(float dtS) override;
	virtual void PrintName() const override;

private:
	Framebuffer* m_pFrameBufferClientArea;
	enum MODE
	{
		OPEN,
		CLOSED,
		EXPANDING,
		COLLAPSING,
	};

	MODE m_Mode;
	float m_Percentage;
	glm::vec4 m_ClientAreaColor;
	std::vector<IExpandableListener*> m_Listeners;
};