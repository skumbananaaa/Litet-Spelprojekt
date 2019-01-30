#pragma once
#include <EnginePch.h>
#include <System/Input.h>
#include <Graphics/GUI/GUIContext.h>
#include <Graphics/Textures/Texture2D.h>

class API GUIObject
{
	friend class GUIManager;

public:
	virtual ~GUIObject();

	bool HasParent() const;
	GUIObject* GetParent() const;

	void Add(GUIObject* parent);
	void Remove(GUIObject* parent);

	virtual float GetWidth() const noexcept;
	virtual float GetHeight() const noexcept;
	float GetX() const noexcept;
	float GetY() const noexcept;
	virtual float GetXInWorld(const GUIObject* child = nullptr) const noexcept;
	virtual float GetYInWorld(const GUIObject* child = nullptr) const noexcept;

	void SetVisible(bool visible) noexcept;
	bool IsVisible() noexcept;

	bool IsDirty() const noexcept;
	bool IsMyChild(const GUIObject* child) const noexcept;

	Texture2D* GetTexture() const noexcept;
	void SetTexture(Texture2D* texture);

protected:
	GUIObject(float x, float y, float width, float height);

	virtual void OnAdded(GUIObject* parent) {};
	virtual void OnRemoved(GUIObject* parent) {};

	virtual void OnUpdate(float dtS) {};
	virtual void OnRender(GUIContext* context);

	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) {};
	virtual void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) {};
	virtual void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) {};

	virtual void OnKeyUp(KEY keycode) {};
	virtual void OnKeyDown(KEY keycode) {};

	virtual void RenderBackgroundTexture(GUIContext* context);
	virtual void RenderChildrensFrameBuffers(GUIContext* context);
	virtual void RenderRealTime(GUIContext* context);
	virtual void ControllRealTimeRenderingForChildPre(GUIContext* context, GUIObject* child);
	virtual void ControllRealTimeRenderingForChildPost(GUIContext* context, GUIObject* child);
	bool ContainsPoint(const glm::vec2& position);
	virtual void PrintName() const = 0;

	template<class T>
	static bool Contains(const std::vector<T*>& list, const T* object)
	{
		for (T* currentObject : list)
		{
			if (currentObject == object)
			{
				return true;
			}
		}
		return false;
	}

	void RequestRepaint();

	static void AddMouseListener(GUIObject* listener);
	static void RemoveMouseListener(GUIObject* listener);

	static void AddRealTimeRenderer(GUIObject* listener);
	static void RemoveRealTimeRenderer(GUIObject* listener);

private:
	void InternalOnUpdate(float dtS);
	void InternalOnRender(GUIContext* context);

	void InternalRootOnRender(GUIContext* context);

	void InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position);

	void RerenderChildren(GUIContext* context);

	GUIObject* m_pParent;
	std::vector<GUIObject*> m_Children;
	std::vector<GUIObject*> m_ChildrenToRemove;
	std::vector<GUIObject*> m_ChildrenToAdd;
	std::vector<GUIObject*> m_ChildrenDirty;
	Framebuffer* m_pFramebuffer;
	glm::vec2 m_Position;
	bool m_IsDirty;
	bool m_IsVisible;
	Texture2D* m_pBackgroundTexture;

	static std::vector<GUIObject*> m_MouseListeners;
	static std::vector<GUIObject*> m_RealTimeRenderers;
};