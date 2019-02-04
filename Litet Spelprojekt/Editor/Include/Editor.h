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
#include <IO/ResourceHandler.h>
#include <IO/WorldSerializer.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>

#define NUM_ROOM_COLORS 6
#define TILE_UNDEFINED_INDEX 0
#define TILE_DOOR_INDEX 1
#define TILE_LADDER_UP 2
#define TILE_LADDER_DOWN 3
#define TILE_SMALLEST_FREE 4

enum EditingMode : uint32
{
	NONE,
	ADD_ROOM,
	EDIT_ROOM,
	DELETE_ROOM,
	ADD_DOOR,
	REMOVE_DOOR
};

class Editor : public Application, public IResourceListener
{
public:
	Editor() noexcept;
	~Editor();

	glm::ivec2 CalculateGridPosition(const glm::vec2& mousePosition) noexcept;
	glm::ivec2 CalculateLowestCorner(const glm::ivec2& firstCorner, const glm::ivec2& secondCorner) noexcept;

	void OnMouseMove(const glm::vec2& position) override;
	void OnMousePressed(MouseButton mousebutton, const glm::vec2& position) override;
	void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;

	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;

	void OnResourcesLoaded() override;

	static void OnButtonReleased(Button* button);
	static Editor* GetEditor();

private:
	IRenderer* m_pRenderer;
	Scene* m_pScene;

	EditingMode m_CurrentEditingMode;

	glm::vec3 m_RoomColors[NUM_ROOM_COLORS];
	uint32 m_LargestIndexUsed;

	bool m_Dragging;
	int32 m_RoomBeingEdited;
	glm::ivec2 m_FirstCorner;

	glm::vec3 m_MouseColor;

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
	Button* m_pButtonAddDoor;
	Button* m_pButtonRemoveDoor;
	Panel* m_pPanelEditor;

	TextView* m_pTextViewMesh;
	Panel* m_pPanelMesh;
	PanelScrollable* m_pPanelScrollableMesh;
};