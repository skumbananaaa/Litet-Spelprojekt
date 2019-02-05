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
#include <Graphics/GUI/SelectionHandler.h>
#include <World/Grid.h>
#include <IO/ResourceHandler.h>
#include <IO/WorldSerializer.h>
#include <Graphics/Textures/TextureHelper.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>

#define NUM_GRID_LEVELS 6
#define NUM_BOAT_LEVELS 3

#define MAX_NUM_ROOMS 32
#define TILE_DOOR_INDEX 0
#define TILE_NON_WALKABLE_INDEX 1
#define TILE_SMALLEST_FREE 2

enum EditingMode : uint32
{
	NONE,
	ADD_ROOM,
	EDIT_ROOM,
	DELETE_ROOM,
	ADD_DOOR,
	REMOVE_DOOR
};

class Editor : public Application, public IResourceListener, public ISelectionListener
{
public:
	Editor() noexcept;
	~Editor();

	void NormalizeTileIndexes() noexcept;
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
	void OnSelected(const SelectionHandler* handler, ISelectable* selection) override;
	void OnDeselected(const SelectionHandler* handler, ISelectable* selection) override;

	static void OnButtonReleased(Button* button);
	static Editor* GetEditor();

private:
	IRenderer* m_pRenderer;
	Scene** m_ppScenes;

	EditingMode m_CurrentEditingMode;

	uint32 m_TileTints[MAX_NUM_ROOMS];
	uint32 m_TileColors[MAX_NUM_ROOMS];
	uint32 m_LargestIndexUsed;
	uint32 m_CurrentGridIndex;

	bool m_Dragging;
	int32 m_RoomBeingEdited;
	glm::ivec2 m_FirstCorner;

	uint32 m_MouseMaterial;

	std::vector<UniformBuffer*> m_GameObjectUniforms;

	Grid** m_ppGrids;

	Button* m_pButtonSave;
	Button* m_pButtonLoad;
	Button* m_pButtonRoom;
	Button* m_pButtonMesh;
	Panel* m_pPanelTop;

	SelectionHandler m_SelectionHandlerFloor;
	TextView* m_pTextViewFloor;
	Button* m_pButtonFloor1;
	Button* m_pButtonFloor2;
	Button* m_pButtonFloor3;
	Panel* m_pPanelFloor;

	SelectionHandler m_SelectionHandlerRoom;
	TextView* m_pTextViewEditor;
	Button* m_pButtonAddRoom;
	Button* m_pButtonEditRoom;
	Button* m_pButtonRemoveRoom;
	Button* m_pButtonAddDoor;
	Button* m_pButtonRemoveDoor;
	Panel* m_pPanelEditor;

	Panel* m_pPanelMesh;
	Button* m_pButtonAddMesh;
	Button* m_pButtonEditMesh;
	PanelScrollable* m_pPanelScrollableAddMesh;
	PanelScrollable* m_pPanelScrollableEditMesh;
};