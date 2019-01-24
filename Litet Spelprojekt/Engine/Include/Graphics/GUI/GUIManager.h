#pragma once
#include <System/Input.h>
#include <Graphics/Renderers/GLContext.h>

#include <algorithm> 

class GUIObject;

class API GUIManager
{
	friend class Application;

public:
	GUIManager();

	void AddGUIObject(GUIObject* object);
	void RemoveGUIObject(GUIObject* object);

	void MarkDepthDirty();

private:
	std::vector<GUIObject*> m_GUIObjects;
	std::vector<GUIObject*> m_GUIObjectsToRemove;
	std::vector<GUIObject*> m_GUIObjectsToAdd;
	std::vector<GUIObject*> m_GUIObjectsDirty;

	bool m_OrderIsDirty;

	bool ContainsGUIObject(const std::vector<GUIObject*>& list, GUIObject* object);
	void RequestRepaint(GUIObject* object);

	virtual void OnUpdate(float dtS);
	virtual void OnRender(const GLContext* context);

	virtual void OnMousePressed(MouseButton mousebutton);
	virtual void OnMouseReleased(MouseButton mousebutton);
	virtual void OnMouseMove(const glm::vec2& position);

	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);
};