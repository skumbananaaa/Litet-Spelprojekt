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
#include <Graphics/GUI/ProgressBar.h>
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

#define MAX_NUM_ROOMS 128
#define TILE_DOOR_INDEX 0
#define TILE_NON_WALKABLE_INDEX 1
#define TILE_SMALLEST_FREE 2

#define ELEMENT_HEIGHT 50

enum Direction : uint32
{
	FORWARD,
	RIGHT,
	BACKWARD,
	LEFT
};

enum EditingMode : uint32
{
	NONE,
	ADD_ROOM,
	EDIT_ROOM,
	DELETE_ROOM,
	ADD_DOOR,
	REMOVE_DOOR,
	ADD_STAIRS,
	REMOVE_STAIRS
};

class Editor : public Application, public ISelectionListener
{
public:
	Editor() noexcept;
	~Editor();

	void NormalizeTileIndexes() noexcept;
	glm::ivec2 CalculateGridPosition(const glm::vec2& mousePosition) noexcept;
	glm::ivec2 CalculateLowestCorner(const glm::ivec2& firstCorner, const glm::ivec2& secondCorner) noexcept;

	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;
	void OnMousePressed(MouseButton mousebutton, const glm::vec2& position) override;
	void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;

	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;

	void OnResourceLoading(const std::string&, float percentage) override;
	void OnResourcesLoaded() override;
	void OnSelected(const SelectionHandler* pHandler, ISelectable* pSelection) override;
	void OnDeselected(const SelectionHandler* pHandler, ISelectable* pSelection) override;

	void CreateWalls();
	WorldLevel** CreateWorldLevels(std::vector<glm::ivec3>& stairs);
	void CreateMesh(GameObject* pGameObject, const std::string& name, int32 index = 0, int32 total = 0);
	void ClearLevels();
	glm::vec3 CalculateMeshPosition(const glm::vec3& position) noexcept;

	uint32 GetCurrentBoatLevel();
	Scene* GetCurrentScene();
	void GetGameObjects(std::vector<GameObject*>& list, int32 level = -1);

	glm::vec3 GetDirectionBasedOnCamera(Direction direction);

	static void OnButtonReleased(Button* button);
	static Editor* GetEditor();

private:
	IRenderer* m_pRenderer;
	Scene** m_ppScenes;
	float m_CameraZoom;

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

	std::vector<GameObject*> m_Walls[NUM_BOAT_LEVELS];

	ProgressBar* m_pLoadingBar;
	TextView* m_pTextViewFile;

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
	Button* m_pButtonAddStairs;
	Button* m_pButtonRemoveStairs;
	Panel* m_pPanelEditor;

	SelectionHandler m_SelectionHandlerMesh;
	SelectionHandler m_SelectionHandlerMeshEdit;
	Panel* m_pPanelMesh;
	Button* m_pButtonAddMesh;
	Button* m_pButtonEditMesh;
	PanelScrollable* m_pPanelScrollableAddMesh;
	PanelScrollable* m_pPanelScrollableEditMesh;
};