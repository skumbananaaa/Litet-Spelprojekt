#include <Graphics/GUI/GUIManager.h>
#include <Graphics/GUI/GUIObject.h>


GUIManager::GUIManager() : m_OrderIsDirty(false)
{

}

void GUIManager::AddGUIObject(GUIObject* object)
{
	if (!ContainsGUIObject(m_GUIObjects, object) && !ContainsGUIObject(m_GUIObjectsToAdd, object))
	{
		m_GUIObjectsToAdd.push_back(object);
	}
}

void GUIManager::RemoveGUIObject(GUIObject* object)
{
	if (ContainsGUIObject(m_GUIObjects, object))
	{
		m_GUIObjectsToRemove.push_back(object);
	}
}

void GUIManager::MarkDepthDirty()
{
	m_OrderIsDirty = true;
}

bool GUIManager::ContainsGUIObject(const std::vector<GUIObject*>& list, GUIObject* object)
{
	for (GUIObject* guiObject : list)
	{
		if (guiObject == object)
		{
			return true;
		}
	}
	return false;
}

void GUIManager::RequestRepaint(GUIObject* object)
{
	if (!ContainsGUIObject(m_GUIObjectsDirty, object))
	{
		m_GUIObjectsDirty.push_back(object);
	}
}

void GUIManager::OnUpdate(float dtS)
{
	for (GUIObject* objectToRemove : m_GUIObjectsToRemove)
	{
		int32 counter = 0;
		for (GUIObject* object : m_GUIObjects)
		{
			if (objectToRemove == object)
			{
				m_GUIObjects.erase(m_GUIObjects.begin() + counter);
				objectToRemove->OnRemoved(this);
				std::cout << "GUI Object Removed" << std::endl;
				return;
			}
			counter++;
		}
	}
	for (GUIObject* objectToAdd : m_GUIObjectsToAdd)
	{
		m_GUIObjects.push_back(objectToAdd);
		objectToAdd->OnAdded(this);
		m_OrderIsDirty = true;
		std::cout << "GUI Object Added" << std::endl;
	}
	m_GUIObjectsToRemove.clear();
	m_GUIObjectsToAdd.clear();

	if (m_OrderIsDirty)
	{
		m_OrderIsDirty = false;
		std::sort(m_GUIObjects.begin(), m_GUIObjects.end(), [](const GUIObject* a, const GUIObject* b) -> bool
		{
			return a->GetDepth() < b->GetDepth();
		});
	}

	for (GUIObject* object : m_GUIObjects)
	{
		object->OnUpdate(dtS);
	}
}

void GUIManager::OnRender(const GLContext* context)
{
	for (GUIObject* object : m_GUIObjectsDirty)
	{
		
	}

	for (GUIObject* object : m_GUIObjects)
	{
		object->OnRender(context);
	}
}

void GUIManager::OnMousePressed(MouseButton mousebutton)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnMousePressed(mousebutton);
	}
}

void GUIManager::OnMouseReleased(MouseButton mousebutton)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnMouseReleased(mousebutton);
	}
}

void GUIManager::OnMouseMove(const glm::vec2& position)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnMouseMove(position);
	}
}

void GUIManager::OnKeyUp(KEY keycode)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnKeyUp(keycode);
	}
}

void GUIManager::OnKeyDown(KEY keycode)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnKeyDown(keycode);
	}
}