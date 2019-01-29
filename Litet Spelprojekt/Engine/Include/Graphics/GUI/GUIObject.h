#pragma once
#include <EnginePch.h>
#include <System/Input.h>
#include <Graphics/GUI/GUIContext.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/GUI/IMouseListener.h>

class API GUIObject
{
	friend class GUIManager;

public:
	GUIObject(float x, float y, float width, float height, std::string name);
	virtual ~GUIObject();

	bool HasParent() const;
	GUIObject* GetParent() const;

	void Add(GUIObject* parent);
	void Remove(GUIObject* parent);

	float GetWidth() const noexcept;
	float GetHeight() const noexcept;
	float GetX() const noexcept;
	float GetY() const noexcept;
	float GetXInWorld() const noexcept;
	float GetYInWorld() const noexcept;

	bool IsDirty() const noexcept;

	Texture2D* GetTexture() const noexcept;
	void SetTexture(Texture2D* texture);

protected:
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
	bool ContainsPoint(const glm::vec2& position);

	template<class T>
	static bool Contains(const std::vector<T*>& list, T* object)
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

	static void AddMouseListener(IMouseListener* listener);
	static void RemoveMouseListener(IMouseListener* listener);

private:
	void InternalOnUpdate(float dtS);
	void InternalOnRender(GUIContext* context);

	void InternalRootOnRender(GUIContext* context);

	void InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position);

	void RerenderChildren(GUIContext* context);
	void RenderChildrensFrameBuffers(GUIContext* context);

	GUIObject* m_pParent;
	std::vector<GUIObject*> m_Children;
	std::vector<GUIObject*> m_ChildrenToRemove;
	std::vector<GUIObject*> m_ChildrenToAdd;
	std::vector<GUIObject*> m_ChildrenDirty;
	Framebuffer* m_pFramebuffer;
	glm::vec2 m_Position;
	bool m_IsDirty;
	std::string m_Name;
	Texture2D* m_pBackgroundTexture;

	static std::vector<IMouseListener*> m_MouseListeners;
};