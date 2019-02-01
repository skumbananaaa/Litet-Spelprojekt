#pragma once

#include <System\Application.h>
#include <Graphics\GameObject.h>
#include <Graphics\Buffers\UniformBuffer.h>
#include <Graphics\Camera.h>
#include <Graphics/Renderers/IRenderer.h>
#include <Graphics/Renderers/OrthographicRenderer.h>
#include <Graphics\Scene.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>
#include <Graphics/GUI/PanelScrollable.h>
#include <World/Grid.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>


class Editor : public Application
{
public:
	Editor() noexcept;
	~Editor();

	glm::ivec2 CalculateGridPosition(const glm::vec2& mousePosition) noexcept;
	glm::ivec2 CalculateLowestCorner(const glm::ivec2& firstCorner, const glm::ivec2& secondCorner) noexcept;

	void OnMousePressed(MouseButton mousebutton, const glm::vec2& position) override;
	void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;

	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;

	static void OnButtonReleased(Button* button);
	static Editor* GetEditor();

private:
	IRenderer* m_pRenderer;
	Scene* m_pScene;

	bool m_Dragging;
	glm::ivec2 m_FirstCorner;

	std::vector<UniformBuffer*> m_GameObjectUniforms;

	Grid* m_pGrid;
	UniformBuffer* m_pGridUniform;

	Button* m_pButtonSave;
	Button* m_pButtonLoad;
	Button* m_pButtonRoom;
	Button* m_pButtonMesh;
	Panel* m_pPanelTop;

	TextView* m_pTextViewFloor;
	Button* m_pButtonFloor1;
	Button* m_pButtonFloor2;
	Button* m_pButtonFloor3;
	Panel* m_pPanelFloor;

	TextView* m_pTextViewEditor;
	Button* m_pButtonAdd;
	Button* m_pButtonEdit;
	Button* m_pButtonRemove;
	Panel* m_pPanelEditor;
};