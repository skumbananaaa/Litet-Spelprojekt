#pragma once
#include <EnginePch.h>
#include <System/Input.h>
#include <Graphics/GUI/GUIContext.h>
#include <Graphics/Textures/Texture2D.h>

class GUIObject;

class API IExternalUIRenderer
{
public:
	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) = 0;
};

class API GUIObject
{
	friend class GUIManager;

public:
	virtual ~GUIObject();

	bool HasParent() const noexcept;
	GUIObject* GetParent() const noexcept;

	virtual void Add(GUIObject* parent) noexcept;
	void Remove(GUIObject* parent) noexcept;

	virtual float GetWidth() const noexcept;
	virtual float GetHeight() const noexcept;
	float GetX() const noexcept;
	float GetY() const noexcept;
	virtual float GetXInWorld(const GUIObject* child = nullptr) const noexcept;
	virtual float GetYInWorld(const GUIObject* child = nullptr) const noexcept;

	virtual void SetSize(float width, float height) noexcept;
	virtual void SetPosition(float x, float y) noexcept;

	virtual void SetVisible(bool visible) noexcept;
	virtual bool IsVisible() noexcept;

	bool IsRealtimeRendered() const noexcept;

	bool IsDirty() const noexcept;
	bool IsMyChild(const GUIObject* child) const noexcept;

	Texture2D* GetBackgroundTexture() const noexcept;
	void SetBackgroundTexture(Texture2D* texture);

	const glm::vec4& GetBackgroundColor() const noexcept;
	virtual void SetBackgroundColor(const glm::vec4& color) noexcept;

	virtual bool ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept;
	bool ContainsPoint(const glm::vec2& position) const noexcept;

	virtual bool OwnsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept;
	bool OwnsPoint(const glm::vec2& position) const noexcept;

	virtual void DeleteChildren();
	virtual void SetDeleteAllChildrenOnDestruction(bool deleteAll);
	virtual bool WillDeleteAllChildrenOnDestruction() const noexcept;

	void AddExternalRenderer(IExternalUIRenderer* renderer);
	void RemoveExternalRenderer(IExternalUIRenderer* renderer);

	void SetUserData(void* data);
	void* GetUserData() const;

	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) {};
	virtual void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) {};
	virtual void OnMouseMove(const glm::vec2& position) {};
	virtual void OnMouseScroll(const glm::vec2& position, const glm::vec2& offset) {};

	const std::vector<GUIObject*>& GetChildren() noexcept;
	int32 GetNrOfChildren() const noexcept;

protected:
	GUIObject(float x, float y, float width, float height);

	virtual void OnAdded(GUIObject* parent) {};
	virtual void OnRemoved(GUIObject* parent) {};

	virtual void OnUpdate(float dtS) {};
	virtual void OnRender(GUIContext* context);
	virtual void OnPreRender(GUIContext* context) {};

	virtual void OnKeyUp(KEY keycode) {};
	virtual void OnKeyDown(KEY keycode) {};
	virtual void OnCharFromKey(char c) {};

	virtual void RenderBackgroundTexture(GUIContext* context);
	virtual void RenderChildrensFrameBuffers(GUIContext* context);
	virtual void RenderRealTimePre(GUIContext* context, float x = 0, float y = 0) {};
	virtual void RenderRealTime(GUIContext* context, float x = 0, float y = 0);
	virtual void RenderRealTimePost(GUIContext* context) {};

	virtual void RecreateFrameBuffer(float width, float height);

	virtual void PrintName() const = 0;
	Texture2D* GetDefaultTexture() const;
	virtual const glm::vec4& GetClearColor() const;
	virtual Texture2D* GetClearTexture() const;


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

	const std::vector<GUIObject*>& GetChildrenToAdd() noexcept;
	const std::vector<GUIObject*>& GetChildrenToRemove() noexcept;

	static void AddMouseListener(GUIObject* listener);
	static void RemoveMouseListener(GUIObject* listener);

	static void AddKeyboardListener(GUIObject* listener);
	static void RemoveKeyboardListener(GUIObject* listener);

	void AddRealTimeRenderer();
	void RemoveRealTimeRenderer();

	void InternalRootOnMouseMove(const glm::vec2& position);

	static std::vector<GUIObject*> s_MouseListeners;
	static std::vector<GUIObject*> s_KeyboardListeners;

private:
	void InternalOnUpdate(float dtS);
	void InternalOnRender(GUIContext* context);
	void InternalOnRealtimeRender(GUIContext* context);

	void InternalRootOnRender(GUIContext* context);

	void InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseScroll(const glm::vec2& position, const glm::vec2& offset);

	void InternalRootOnKeyUp(KEY keycode);
	void InternalRootOnKeyDown(KEY keycode);
	void InternalRootOnCharFromKey(char c);

	void RerenderChildren(GUIContext* context);

	static void CreateDefaultTexture();
	static void DeleteDefaultTexture();

	GUIObject* m_pParent;
	std::vector<GUIObject*> m_Children;
	std::vector<GUIObject*> m_ChildrenToRemove;
	std::vector<GUIObject*> m_ChildrenToAdd;
	std::vector<GUIObject*> m_ChildrenDirty;
	std::vector<IExternalUIRenderer*> m_ExternalRenderers;
	Framebuffer* m_pFramebuffer;
	glm::vec2 m_Position;
	bool m_IsDirty;
	bool m_IsVisible;
	Texture2D* m_pBackgroundTexture;
	glm::vec4 m_BackgroundColor;
	bool m_DeleteAll;
	bool m_IsRealtime;
	void* m_pUserData;

	static Texture2D* s_pDefaultTexture;
};