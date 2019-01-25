#pragma once
#include <EnginePch.h>
#include <System/Input.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Renderers/FontRenderer.h>

#include <algorithm> 

class GUIObject;

struct VertexGUI
{
	glm::vec2 position;
	glm::vec2 texCoords;

	inline bool operator==(const VertexGUI& rs) const
	{
		return (position == rs.position) && (texCoords == rs.texCoords);
	}
};

struct PerObjectDataGUI
{
	glm::mat4 projection;
	glm::vec3 color;
};

class API GUIManager
{
	friend class Application;
	friend class GUIObject;

public:
	GUIManager(float width, float height);
	~GUIManager();

	void AddGUIObject(GUIObject* object);
	void RemoveGUIObject(GUIObject* object);

	void MarkDepthDirty();

private:
	std::vector<GUIObject*> m_GUIObjects;
	std::vector<GUIObject*> m_GUIObjectsToRemove;
	std::vector<GUIObject*> m_GUIObjectsToAdd;
	std::vector<GUIObject*> m_GUIObjectsDirty;
	PerObjectDataGUI m_PerObjectDataGUI;
	UniformBuffer* m_pPerObjectUniform;
	ShaderProgram* m_pShaderProgram;
	FontRenderer* m_pFontRenderer;
	bool m_OrderIsDirty;
	unsigned int m_VAO;
	unsigned int m_VBO;
	VertexGUI m_VertexQuad[6];

	bool ContainsGUIObject(const std::vector<GUIObject*>& list, GUIObject* object);
	void RequestRepaint(GUIObject* object);
	void SetVertexQuadData(float x, float y, float width, float height);

	virtual void OnUpdate(float dtS);
	virtual void OnRender(GLContext* context);

	virtual void OnMousePressed(MouseButton mousebutton);
	virtual void OnMouseReleased(MouseButton mousebutton);
	virtual void OnMouseMove(const glm::vec2& position);

	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);
};