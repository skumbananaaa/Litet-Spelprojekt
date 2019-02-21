#include <Graphics/Renderers/DefferedRenderer.h>
#include "..\Include\Editor.h"
#include <World/LightManager.h>

Editor::Editor() noexcept : Application(false, 1600, 900, "../Game/"),
	m_SelectionHandlerFloor(true),
	m_SelectionHandlerRoom(false),
	m_SelectionHandlerMesh(true),
	m_SelectionHandlerMeshEdit(false),
	m_pOriginalMaterial(nullptr)
{
	std::cout << "Editor" << std::endl;
	m_MouseMaterial = MATERIAL::WHITE;
	//GetGraphicsContext().Disable(Cap::CULL_FACE);

	m_pTextViewFile = new TextView((GetWindow().GetWidth() - 300) / 2, (GetWindow().GetHeight() - 50) / 2 + 50, 300, 50, "Loading...");
	m_pLoadingBar = new ProgressBar((GetWindow().GetWidth() - 300) / 2, (GetWindow().GetHeight() - 50) / 2, 300, 50);

	GetGUIManager().Add(m_pTextViewFile);
	GetGUIManager().Add(m_pLoadingBar);
}

Editor::~Editor()
{
	m_SelectionHandlerFloor.Release();
	m_SelectionHandlerRoom.Release();
	m_SelectionHandlerMesh.Release();
	m_SelectionHandlerMeshEdit.Release();

	Delete(m_pRenderer);

	ClearLevels();

	DeleteArr(m_ppScenes);
	DeleteArr(m_ppGrids);

	Delete(m_pPanelTop);
	Delete(m_pPanelFloor);
	Delete(m_pPanelEditor);

	Delete(m_pTextViewFile);
	Delete(m_pLoadingBar);
}

void Editor::OnResourceLoading(const std::string& file, float percentage)
{
	m_pTextViewFile->SetText("Loading: " + file);
	m_pLoadingBar->SetPercentage(percentage);
}

void Editor::OnResourcesLoaded()
{
	GetGUIManager().Remove(m_pTextViewFile);
	GetGUIManager().Remove(m_pLoadingBar);

	Camera* pCameraPersp = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	pCameraPersp->CreatePerspective(glm::radians<float>(90.0f), GetWindow().GetAspectRatio(), 0.01f, 100.0f);
	pCameraPersp->UpdateFromPitchYaw();

	Camera* pCameraOrth = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians<float>(-90.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	m_CameraZoom = 1.0f;
	pCameraOrth->CreateOrthographic(30.0f * GetWindow().GetAspectRatio() * m_CameraZoom, 30.0f * m_CameraZoom, 0.01f, 100.0f);
	pCameraOrth->UpdateFromPitchYaw();

	m_ppScenes = new Scene*[NUM_BOAT_LEVELS];
	m_ppGrids = new Grid*[NUM_GRID_LEVELS];

	for (uint32 i = 0; i < NUM_BOAT_LEVELS; i++)
	{
		m_ppScenes[i] = new Scene();
		m_ppScenes[i]->SetCamera(pCameraPersp, 0);
		m_ppScenes[i]->SetCamera(pCameraOrth, 1);
	}

	m_RoomBeingEdited = -1;
	m_CurrentGridIndex = 0;
	m_LargestIndexUsed = TILE_SMALLEST_FREE - 1;

	m_Dragging = false;
	m_CurrentEditingMode = NONE;


	m_pPanelTop = new Panel(0, GetWindow().GetHeight() - 70, GetWindow().GetWidth(), 70);
	m_pButtonSave = new Button(10, 10, 140, 50, "Save", nullptr, OnButtonReleased);
	m_pButtonLoad = new Button(160, 10, 140, 50, "Load", nullptr, OnButtonReleased);
	m_pButtonRoom = new Button(310, 10, 140, 50, "Room Editor", nullptr, OnButtonReleased);
	m_pButtonMesh = new Button(460, 10, 140, 50, "Mesh Editor", nullptr, OnButtonReleased);
	m_pPanelTop->Add(m_pButtonSave);
	m_pPanelTop->Add(m_pButtonLoad);
	m_pPanelTop->Add(m_pButtonRoom);
	m_pPanelTop->Add(m_pButtonMesh);
	m_pPanelTop->SetDeleteAllChildrenOnDestruction(true);


	m_pPanelEditor = new Panel(GetWindow().GetWidth() - 200, (GetWindow().GetHeight() - 650) / 2, 200, 650);
	m_pTextViewEditor = new TextView(0, m_pPanelEditor->GetHeight() - 50, m_pPanelEditor->GetWidth(), 50, "Room Tool", true);
	m_pButtonAddRoom = new Button(10, m_pPanelEditor->GetHeight() - 100, m_pPanelEditor->GetWidth() - 20, 50, "New Room");
	m_pButtonEditRoom = new Button(10, m_pPanelEditor->GetHeight() - 160, m_pPanelEditor->GetWidth() - 20, 50, "Edit Room");
	m_pButtonRemoveRoom = new Button(10, m_pPanelEditor->GetHeight() - 220, m_pPanelEditor->GetWidth() - 20, 50, "Delete Room");
	m_pButtonAddDoor = new Button(10, m_pPanelEditor->GetHeight() - 280, m_pPanelEditor->GetWidth() - 20, 50, "Add Door");
	m_pButtonRemoveDoor = new Button(10, m_pPanelEditor->GetHeight() - 340, m_pPanelEditor->GetWidth() - 20, 50, "Remove Door");
	m_pButtonAddStairs = new Button(10, m_pPanelEditor->GetHeight() - 400, m_pPanelEditor->GetWidth() - 20, 50, "Add Stairs");
	m_pButtonRemoveStairs = new Button(10, m_pPanelEditor->GetHeight() - 460, m_pPanelEditor->GetWidth() - 20, 50, "Remove Stairs");
	m_pButtonAddRoom->SetUserData(reinterpret_cast<void*>(ADD_ROOM));
	m_pButtonEditRoom->SetUserData(reinterpret_cast<void*>(EDIT_ROOM));
	m_pButtonRemoveRoom->SetUserData(reinterpret_cast<void*>(DELETE_ROOM));
	m_pButtonAddDoor->SetUserData(reinterpret_cast<void*>(ADD_DOOR));
	m_pButtonRemoveDoor->SetUserData(reinterpret_cast<void*>(REMOVE_DOOR));
	m_pButtonAddStairs->SetUserData(reinterpret_cast<void*>(ADD_STAIRS));
	m_pButtonRemoveStairs->SetUserData(reinterpret_cast<void*>(REMOVE_STAIRS));
	m_pPanelEditor->Add(m_pTextViewEditor);
	m_pPanelEditor->Add(m_pButtonAddRoom);
	m_pPanelEditor->Add(m_pButtonEditRoom);
	m_pPanelEditor->Add(m_pButtonRemoveRoom);
	m_pPanelEditor->Add(m_pButtonAddDoor);
	m_pPanelEditor->Add(m_pButtonRemoveDoor);
	m_pPanelEditor->Add(m_pButtonAddStairs);
	m_pPanelEditor->Add(m_pButtonRemoveStairs);
	m_pPanelEditor->SetDeleteAllChildrenOnDestruction(true);
	m_SelectionHandlerRoom.AddSelectionListener(this);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonAddRoom);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonEditRoom);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonRemoveRoom);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonAddDoor);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonRemoveDoor);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonAddStairs);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonRemoveStairs);


	std::vector<std::string> gameObjects;
	ResourceHandler::QuaryGameObjectTypes(gameObjects);
	m_pPanelMesh = new Panel(GetWindow().GetWidth() - 200, (GetWindow().GetHeight() - 650) / 2, 200, 650);
	m_pButtonAddMesh = new Button(10, m_pPanelMesh->GetHeight() - 60, 85, 50, "New");
	m_pButtonEditMesh = new Button(105, m_pPanelMesh->GetHeight() - 60, 85, 50, "Edit");
	m_pPanelScrollableAddMesh = new PanelScrollable(10, 10, m_pPanelMesh->GetWidth() - 20, m_pPanelMesh->GetHeight() - m_pButtonAddMesh->GetHeight() - 30, m_pPanelMesh->GetWidth() - 20, ELEMENT_HEIGHT * gameObjects.size());
	m_pPanelScrollableEditMesh = new PanelScrollable(10, 10, m_pPanelMesh->GetWidth() - 20, m_pPanelMesh->GetHeight() - m_pButtonAddMesh->GetHeight() - 30, m_pPanelMesh->GetWidth() - 20, m_pPanelMesh->GetHeight() - m_pButtonAddMesh->GetHeight() - 30);
	m_pPanelMesh->Add(m_pButtonAddMesh);
	m_pPanelMesh->Add(m_pButtonEditMesh);
	m_pPanelMesh->Add(m_pPanelScrollableAddMesh);
	m_pPanelMesh->Add(m_pPanelScrollableEditMesh);
	m_pPanelScrollableAddMesh->SetVisible(false);
	m_pPanelMesh->SetVisible(false);
	m_pPanelMesh->SetDeleteAllChildrenOnDestruction(true);
	m_SelectionHandlerMesh.AddSelectionListener(this);
	m_SelectionHandlerMesh.AddSelectable(m_pButtonAddMesh);
	m_SelectionHandlerMesh.AddSelectable(m_pButtonEditMesh);

	m_SelectionHandlerMeshEdit.AddSelectionListener(this);


	m_pPanelFloor = new Panel(0, GetWindow().GetHeight() / 4, 160, GetWindow().GetHeight() / 2);
	m_pTextViewFloor = new TextView(0, m_pPanelFloor->GetHeight() - 50, m_pPanelFloor->GetWidth(), 50, "Select Floor", true);
	m_pButtonFloor1 = new Button(10, m_pPanelFloor->GetHeight() - 100, 140, 50, "Floor 1");
	m_pButtonFloor2 = new Button(10, m_pPanelFloor->GetHeight() - 160, 140, 50, "Floor 2");
	m_pButtonFloor3 = new Button(10, m_pPanelFloor->GetHeight() - 220, 140, 50, "Floor 3");
	m_pButtonFloor1->SetUserData(reinterpret_cast<void*>(0));
	m_pButtonFloor2->SetUserData(reinterpret_cast<void*>(1));
	m_pButtonFloor3->SetUserData(reinterpret_cast<void*>(2));
	m_pPanelFloor->Add(m_pTextViewFloor);
	m_pPanelFloor->Add(m_pButtonFloor1);
	m_pPanelFloor->Add(m_pButtonFloor2);
	m_pPanelFloor->Add(m_pButtonFloor3);
	m_pPanelFloor->SetDeleteAllChildrenOnDestruction(true);
	m_SelectionHandlerFloor.AddSelectionListener(this);
	m_SelectionHandlerFloor.AddSelectable(m_pButtonFloor1);
	m_SelectionHandlerFloor.AddSelectable(m_pButtonFloor2);
	m_SelectionHandlerFloor.AddSelectable(m_pButtonFloor3);


	GetGUIManager().Add(m_pPanelTop);
	GetGUIManager().Add(m_pPanelFloor);
	GetGUIManager().Add(m_pPanelEditor);
	GetGUIManager().Add(m_pPanelMesh);

	for (int32 i = 0; i < gameObjects.size(); i++)
	{
		Button* button = new Button(4, m_pPanelScrollableAddMesh->GetClientHeight() - (i + 1) * ELEMENT_HEIGHT, m_pPanelScrollableAddMesh->GetClientWidth() - 8, ELEMENT_HEIGHT - 4, gameObjects[i], nullptr, OnButtonReleased);
		button->SetTextCentered(false);
		button->SetUserData(reinterpret_cast<void*>(i));
		m_pPanelScrollableAddMesh->Add(button);
	}

	float hDelta = 360.0f / (float)MAX_NUM_ROOMS;
	uint32 sAccumulator = 0;
	uint32 vAccumulator = 0;
	float currentH = 0.0f;
	float currentS = 0.3f;
	float currentV = 0.3f;
	for (uint32 i = 0; i < MAX_NUM_ROOMS; i++)
	{
		m_TileColors[i] = ResourceHandler::RegisterMaterial(HSVA2RGBA(glm::vec4(currentH, currentS, currentV, 1.0f)), 256.0f);
		m_TileTints[i] = ResourceHandler::RegisterMaterial(HSVA2RGBA(glm::vec4(currentH, currentS, currentV * 0.5f, 1.0f)), 256.0f);
		currentH += hDelta;
		sAccumulator += 1;
		vAccumulator += 2;
		currentS = 0.3f + 0.7f * (float)(sAccumulator % 16) / 15.0f;
		currentV = 0.3f + 0.7f * (float)(vAccumulator % 16) / 15.0f;
	}

	m_pRenderer = new OrthographicRenderer();

	const int32 gridWidth = 40;
	const int32 gridHeight = 10;

	for (uint32 i = 0; i < NUM_GRID_LEVELS; i++)
	{
		//Create one grid for each grid level
		m_ppGrids[i] = new Grid(MATERIAL::BLACK, glm::ivec2(gridHeight, gridWidth), glm::vec3(-gridHeight / 2.0f, 0.0f, -gridWidth / 2.0f));

		for (uint32 x = 0; x < m_ppGrids[i]->GetSize().x; x++)
		{
			for (uint32 y = 0; y < m_ppGrids[i]->GetSize().y; y++)
			{
				Tile* pTile = m_ppGrids[i]->GetTile(glm::ivec2(x, y));
				pTile->SetID(TILE_NON_WALKABLE_INDEX);
				m_ppScenes[i / 2]->AddGameObject(pTile);
			}
		}
	}

	std::cout << "Resources Loaded!" << std::endl;
}

void Editor::OnSelected(const SelectionHandler* pHandler, ISelectable* pSelection)
{
	if (pHandler == &m_SelectionHandlerFloor)
	{
		Button* pButton = (Button*)pSelection;
		uint32 floorIndex = reinterpret_cast<uint32>(pButton->GetUserData());
		std::cout << "New Floor Index: " << floorIndex << std::endl;

		m_CurrentGridIndex = 2 * floorIndex;
		m_Dragging = false;
		m_RoomBeingEdited = -1;
		m_MouseMaterial = MATERIAL::WHITE;
		
		//Reload All meshes in the panel for the new floor
		std::vector<GameObject*> gameObjects;
		GetGameObjects(gameObjects);
		m_SelectionHandlerMeshEdit.Release();
		m_SelectionHandlerMeshEdit.AddSelectionListener(this);
		m_pPanelScrollableEditMesh->DeleteChildren();
		m_pPanelScrollableEditMesh->SetClientSize(m_pPanelScrollableEditMesh->GetClientWidth(), gameObjects.size() * ELEMENT_HEIGHT + 4);
		for (int64 i = 0; i < gameObjects.size(); i++)
		{
			std::string name = ResourceHandler::GetGameObjectName(gameObjects[i]->GetTypeId());
			Button* button = new Button(4, m_pPanelScrollableEditMesh->GetClientHeight() - (i + 1) * ELEMENT_HEIGHT, m_pPanelScrollableEditMesh->GetClientWidth() - 8, ELEMENT_HEIGHT - 4, name);
			button->SetTextCentered(false);
			button->SetUserData(gameObjects[i]);
			m_pPanelScrollableEditMesh->Add(button);
			m_SelectionHandlerMeshEdit.AddSelectable(button);
		}
	}
	else if (pHandler == &m_SelectionHandlerRoom)
	{
		Button* pButton = (Button*)pSelection;
		EditingMode newMode = (EditingMode)reinterpret_cast<uint32>(pButton->GetUserData());
		std::cout << "New Editing Mode: " << newMode << std::endl;

		m_CurrentEditingMode = newMode;
		m_Dragging = false;
		m_RoomBeingEdited = -1;
		m_MouseMaterial = MATERIAL::WHITE;
	}
	else if (pHandler == &m_SelectionHandlerMesh)
	{
		if (pSelection == m_pButtonAddMesh)
		{
			m_pPanelScrollableAddMesh->SetVisible(true);
			m_pPanelScrollableEditMesh->SetVisible(false);
		} 
		else if (pSelection == m_pButtonEditMesh)
		{
			m_pPanelScrollableAddMesh->SetVisible(false);
			m_pPanelScrollableEditMesh->SetVisible(true);
		}
	}
	else if (pHandler == &m_SelectionHandlerMeshEdit)
	{
		Button* pButton = (Button*)pSelection;
		GameObject* pGameObject = (GameObject*)pButton->GetUserData();
		m_pOriginalMaterial = pGameObject->GetMaterial();
		pGameObject->SetMaterial(MATERIAL::WHITE);
		GetCurrentScene()->GetCamera().SetLookAt(pGameObject->GetPosition());
	}
}

void Editor::OnDeselected(const SelectionHandler* pHandler, ISelectable* pSelection)
{
	if (pHandler == &m_SelectionHandlerFloor)
	{
		Button* pButton = (Button*)pSelection;
		uint32 floorIndex = reinterpret_cast<uint32>(pButton->GetUserData());
		std::cout << "Last Floor Index: " << floorIndex << std::endl;
	}
	else if (pHandler == &m_SelectionHandlerRoom)
	{
		Button* pButton = (Button*)pSelection;
		EditingMode newMode = (EditingMode)reinterpret_cast<uint32>(pButton->GetUserData());
		std::cout << "Last Editing Mode: " << newMode << std::endl;
		m_CurrentEditingMode = NONE;
	}
	else if (pHandler == &m_SelectionHandlerMeshEdit)
	{
		Button* pButton = (Button*)pSelection;
		GameObject* pGameObject = (GameObject*)pButton->GetUserData();
		pGameObject->SetMaterial(ResourceHandler::GetMaterial(m_pOriginalMaterial));
	}
}

void Editor::CreateWalls()
{
	for (int i = 0; i < NUM_BOAT_LEVELS; i++)
	{
		for (int j = 0; j < m_Walls[i].size(); j++)
		{
			for (int k = 0; k < m_ppScenes[i]->GetGameObjects().size(); k++)
			{
				if (m_ppScenes[i]->GetGameObjects()[k] == m_Walls[i][j])
				{
					m_ppScenes[i]->GetGameObjects().erase(m_ppScenes[i]->GetGameObjects().begin() + k);
					break;
				}
			}
			delete m_Walls[i][j];
		}
		m_Walls[i].clear();
	}

	std::vector<glm::ivec3> stairs;
	std::vector<glm::ivec3> doors;
	WorldLevel** ppWorldLevels = CreateWorldLevels(stairs, doors);

	for (int level = 0; level < NUM_GRID_LEVELS; level += 2)
	{
		ppWorldLevels[level]->GenerateRooms();
		float halfWidth = ppWorldLevels[level]->GetSizeX() / 2;
		float halfHeight = ppWorldLevels[level]->GetSizeZ() / 2;
		glm::vec4 wall;

		for (int i = 0; i < ppWorldLevels[level]->GetNrOfWalls(); i++)
		{
			wall = ppWorldLevels[level]->GetWall(i);
			GameObject* pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::WHITE);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetPosition(glm::vec3(wall.x - halfWidth, 1.0f, wall.y - halfHeight));
			pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
			pGameObject->UpdateTransform();
			m_ppScenes[level / 2]->AddGameObject(pGameObject);
			m_Walls[level / 2].push_back(pGameObject);
		}
	}
}

WorldLevel** Editor::CreateWorldLevels(std::vector<glm::ivec3>& stairs, std::vector<glm::ivec3>& doors)
{
	WorldLevel** ppWorldLevels = new WorldLevel*[NUM_GRID_LEVELS];

	for (uint32 gridId = 0; gridId < NUM_GRID_LEVELS; gridId++)
	{
		uint32 levelSizeX = m_ppGrids[gridId]->GetSize().x + 2;
		uint32 levelSizeY = m_ppGrids[gridId]->GetSize().y + 2;
		uint32* pLevel = new uint32[levelSizeX * levelSizeY];

		for (uint32 x = 0; x < levelSizeX; x++)
		{
			for (uint32 y = 0; y < levelSizeY; y++)
			{
				if (x == 0 || x == levelSizeX - 1 || y == 0 || y == levelSizeY - 1)
				{
					pLevel[x * levelSizeY + y] = TILE_NON_WALKABLE_INDEX;
				}
				else
				{
					Tile* pTile = m_ppGrids[gridId]->GetTile(glm::ivec2(x - 1, y - 1));
					pLevel[x * levelSizeY + y] = pTile->GetID();

					if (pTile->HasStairs())
					{
						stairs.push_back(glm::ivec3(x, gridId, y));
					}
					else if (pTile->HasDoor())
					{
						doors.push_back(glm::ivec3(x, gridId, y));
					}
				}
			}
		}

		ppWorldLevels[gridId] = new WorldLevel(gridId, pLevel, levelSizeX, levelSizeY);

		for (uint32 doorId = 0; doorId < doors.size(); doorId++)
		{
			ppWorldLevels[gridId]->GetLevelData()[doors[doorId].x][doors[doorId].z].HasDoor = true;
		}
	}
	return ppWorldLevels;
}

void Editor::CreateMesh(GameObject* pGameObject, const std::string& name, int32 index, int32 total)
{
	//Create new object
	std::vector<GameObject*> gameObjects;
	GetGameObjects(gameObjects);
	int32 size = gameObjects.size();
	int32 pos = gameObjects.size();
	if (total != 0)
	{
		size = total;
		pos = index + 1;
	}
	m_pPanelScrollableEditMesh->SetClientSize(m_pPanelScrollableEditMesh->GetClientWidth(), size * ELEMENT_HEIGHT + 4);
	Button* pButton = new Button(4, m_pPanelScrollableEditMesh->GetClientHeight() - pos * ELEMENT_HEIGHT, m_pPanelScrollableEditMesh->GetClientWidth() - 8, ELEMENT_HEIGHT - 4, name);
	pButton->SetTextCentered(false);
	pButton->SetUserData(pGameObject);
	m_pPanelScrollableEditMesh->Add(pButton);
	m_SelectionHandlerMeshEdit.AddSelectable(pButton);

	//Arange objects
	if (total == 0)
	{
		for (int32 i = 0; i < m_pPanelScrollableEditMesh->GetChildren().size(); i++)
		{
			GUIObject* pObject = m_pPanelScrollableEditMesh->GetChildren()[i];
			pObject->SetPosition(pObject->GetX(), m_pPanelScrollableEditMesh->GetClientHeight() - (i + 1) * ELEMENT_HEIGHT);
		}
	}
	
	if (pos == total || total == 0)
	{
		m_SelectionHandlerMesh.OnSelected(m_pButtonEditMesh);
		m_SelectionHandlerMeshEdit.OnSelected(pButton);
	}
}

void Editor::ClearLevels()
{
	m_SelectionHandlerMeshEdit.Release();
	m_SelectionHandlerMeshEdit.AddSelectionListener(this);
	m_pPanelScrollableEditMesh->DeleteChildren();

	for (uint32 i = 0; i < NUM_BOAT_LEVELS; i++)
	{
		m_Walls[i].clear();

		if (i > 0)
		{
			m_ppScenes[i]->SetCamera(nullptr, 0);
			m_ppScenes[i]->SetCamera(nullptr, 1);
		}
		Delete(m_ppScenes[i]);
	}

	for (uint32 i = 0; i < NUM_GRID_LEVELS; i++)
	{
		Delete(m_ppGrids[i]);
	}
}

uint32 Editor::GetCurrentBoatLevel()
{
	return m_CurrentGridIndex / 2;
}

Scene* Editor::GetCurrentScene()
{
	return m_ppScenes[GetCurrentBoatLevel()];
}

void Editor::GetGameObjects(std::vector<GameObject*>& list, int32 level)
{
	Scene* scene = nullptr;
	if (level == -1)
	{
		scene = GetCurrentScene();
	}
	else
	{
		scene = m_ppScenes[level];
	}
	for (GameObject* gameObject : scene->GetGameObjects())
	{
		if (gameObject->GetTypeId() != -1)
		{
			list.push_back(gameObject);
		}
	}
}

glm::vec3 Editor::GetDirectionBasedOnCamera(Direction direction)
{
	Camera& camera = GetCurrentScene()->GetCamera();
	float yaw = camera.GetYaw();
	while (yaw > glm::two_pi<float>())
	{
		yaw -= glm::two_pi<float>();
	}
	while (yaw < 0)
	{
		yaw += glm::two_pi<float>();
	}
	yaw = fmod(yaw + glm::quarter_pi<float>(), glm::two_pi<float>());
	int dir = yaw / glm::half_pi<float>();
	
	std::cout << "dir = " << dir << std::endl;

	switch (dir)
	{
		case 0:
		{
			switch (direction)
			{
				case FORWARD: return glm::vec3(1, 0, 0);
				case RIGHT: return glm::vec3(0, 0, 1);
				case BACKWARD: return glm::vec3(-1, 0, 0);
				case LEFT: return glm::vec3(0, 0, -1);
			}
		}
		case 1:
		{
			switch (direction)
			{
				case FORWARD: return glm::vec3(0, 0, 1);
				case RIGHT: return glm::vec3(-1, 0, 0);
				case BACKWARD: return glm::vec3(0, 0, -1);
				case LEFT: return glm::vec3(1, 0, 0);
			}
		}
		case 2:
		{
			switch (direction)
			{
				case FORWARD: return glm::vec3(-1, 0, 0);
				case RIGHT: return glm::vec3(0, 0, -1);
				case BACKWARD: return glm::vec3(1, 0, 0);
				case LEFT: return glm::vec3(0, 0, 1);
			}
		}
		case 3:
		{
			switch (direction)
			{
				case FORWARD: return glm::vec3(0, 0, -1);
				case RIGHT: return glm::vec3(1, 0, 0);
				case BACKWARD: return glm::vec3(0, 0, 1);
				case LEFT: return glm::vec3(-1, 0, 0);
			}
		}
	}
	//std::cout << "Error! Camera dir = " << dir << " This is not a good thing" << std::endl;
	return glm::vec3();
}

void Editor::NormalizeTileIndexes() noexcept
{
	std::cout << "Normalizing Tile Indexes" << std::endl;
	bool indexesExisting[MAX_NUM_ROOMS];
	uint32 indexesMissingBefore[MAX_NUM_ROOMS];

	for (uint32 i = 0; i < MAX_NUM_ROOMS; i++)
	{
		indexesExisting[i] = false;
		indexesMissingBefore[i] = 0;
	}

	for (uint32 gridId = 0; gridId < NUM_GRID_LEVELS; gridId++)
	{
		for (uint32 x = 0; x < m_ppGrids[gridId]->GetSize().x; x++)
		{
			for (uint32 y = 0; y < m_ppGrids[gridId]->GetSize().y; y++)
			{
				Tile* pTile = m_ppGrids[gridId]->GetTile(glm::ivec2(x, y));
				int32 index = pTile->GetID() - TILE_SMALLEST_FREE;

				if (index >= 0)
				{
					indexesExisting[index] = true;
				}
			}
		}
	}

	uint32 indexesMissing = 0;
	bool someIndexExists = false;
	for (uint32 i = 0; i < MAX_NUM_ROOMS; i++)
	{
		if (!indexesExisting[i])
		{
			indexesMissing++;
		}
		else
		{
			someIndexExists = true;
			m_LargestIndexUsed = i + TILE_SMALLEST_FREE - indexesMissing;
		}

		indexesMissingBefore[i] = indexesMissing;
	}

	if (!someIndexExists)
	{
		m_LargestIndexUsed = TILE_SMALLEST_FREE - 1;
	}

	std::cout << "New Largest Index Used: " << m_LargestIndexUsed << std::endl;

	for (uint32 gridId = 0; gridId < NUM_GRID_LEVELS; gridId++)
	{
		for (uint32 x = 0; x < m_ppGrids[gridId]->GetSize().x; x++)
		{
			for (uint32 y = 0; y < m_ppGrids[gridId]->GetSize().y; y++)
			{
				Tile* pTile = m_ppGrids[gridId]->GetTile(glm::ivec2(x, y));
				int32 index = pTile->GetID() - TILE_SMALLEST_FREE;

				if (index >= 0)
				{
					uint32 newIndex = pTile->GetID() - indexesMissingBefore[index];
					pTile->SetID(newIndex);

					if (!pTile->HasStairs() && !pTile->HasDoor())
					{
						pTile->SetDefaultMaterial(m_TileColors[(newIndex - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
					}
				}
			}
		}
	}
}

glm::ivec2 Editor::CalculateGridPosition(const glm::vec2& mousePosition) noexcept
{
	glm::vec2 clipSpacePosition(mousePosition.x / static_cast<float>(GetWindow().GetWidth()), mousePosition.y / static_cast<float>(GetWindow().GetHeight()));
	clipSpacePosition = (clipSpacePosition - glm::vec2(0.5f)) * 2.0f;
	glm::vec3 worldPosition = GetCurrentScene()->GetCamera().GetInverseCombinedMatrix() * glm::vec4(clipSpacePosition.x, clipSpacePosition.y, 0.0f, 1.0f);
	float xOffset = static_cast<float>(m_ppGrids[m_CurrentGridIndex]->GetSize().x % 2) / 2.0f;
	float yOffset = static_cast<float>(m_ppGrids[m_CurrentGridIndex]->GetSize().y % 2) / 2.0f;
	glm::ivec2 gridPosition(
		static_cast<uint32>(glm::round(worldPosition.x + xOffset)) + m_ppGrids[m_CurrentGridIndex]->GetSize().x / 2,
		static_cast<uint32>(glm::round(worldPosition.z + yOffset)) + m_ppGrids[m_CurrentGridIndex]->GetSize().y / 2);
	return gridPosition;
}

glm::vec3 Editor::CalculateMeshPosition(const glm::vec3& worldPosition) noexcept
{
	return worldPosition + glm::vec3(static_cast<float>(m_ppGrids[m_CurrentGridIndex]->GetSize().x % 2) / 2.0f, 0, static_cast<float>(m_ppGrids[m_CurrentGridIndex]->GetSize().y % 2) / 2.0f);
}

glm::ivec2 Editor::CalculateLowestCorner(const glm::ivec2& firstCorner, const glm::ivec2& secondCorner) noexcept
{
	glm::ivec2 lowestCorner;
	lowestCorner.x = glm::min(firstCorner.x, secondCorner.x);
	lowestCorner.y = glm::min(firstCorner.y, secondCorner.y);
	return lowestCorner;
}

void Editor::OnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	const float cameraZoomSensitivity = 0.1f;
	GetCurrentScene()->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::Zoom, cameraZoomSensitivity * offset.y);
}

void Editor::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (Input::IsKeyDown(KEY_LEFT_ALT) && !m_pPanelEditor->IsVisible())
	{
		if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
		{
			const float cameraRotationSensitivity = 0.005f;
			glm::vec2 deltaPosition = cameraRotationSensitivity * (position - lastPosition);

			GetCurrentScene()->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, deltaPosition.x);
			GetCurrentScene()->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateY, -deltaPosition.y);
		}

		if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
		{
			const float cameraMoveSensitivityX = 0.5f;
			const float cameraMoveSensitivityY = 0.025f;
			glm::vec2 deltaPosition = cameraMoveSensitivityY * (position - lastPosition);
			glm::vec3 forward(0.0f);
			forward.x = GetCurrentScene()->GetCamera().GetFront().x;
			forward.z = GetCurrentScene()->GetCamera().GetFront().z;
			GetCurrentScene()->GetCamera().MoveWorldCoords(-forward * deltaPosition.y, true);
			GetCurrentScene()->GetCamera().MoveLocalCoords(glm::vec3(cameraMoveSensitivityX * deltaPosition.x, 0.0f, 0.0f), true);
		}
	}
	else
	{
		for (uint32 x = 0; x < m_ppGrids[m_CurrentGridIndex]->GetSize().x; x++)
		{
			for (uint32 y = 0; y < m_ppGrids[m_CurrentGridIndex]->GetSize().y; y++)
			{
				m_ppGrids[m_CurrentGridIndex]->GetTile(glm::ivec2(x, y))->ResetMaterial();
			}
		}

		glm::ivec2 gridPos = CalculateGridPosition(position);
		if (gridPos.x >= 0 && gridPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
			gridPos.y >= 0 && gridPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
		{
			m_ppGrids[m_CurrentGridIndex]->GetTile(gridPos)->SetMaterial(m_MouseMaterial);
		}

		if (m_Dragging)
		{
			glm::ivec2 lowestCorner = CalculateLowestCorner(m_FirstCorner, gridPos);
			glm::ivec2 area = glm::abs(gridPos - m_FirstCorner) + glm::ivec2(1, 1);
			uint32 numTiles = area.x * area.y;

			if (m_CurrentEditingMode == ADD_ROOM)
			{
				for (uint32 i = 0; i < numTiles; i++)
				{
					glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (pTile->GetID() == TILE_NON_WALKABLE_INDEX)
						{
							pTile->SetMaterial(m_TileTints[(m_LargestIndexUsed - TILE_SMALLEST_FREE + 1) % MAX_NUM_ROOMS]);
						}
					}
				}
			}
			else if (m_CurrentEditingMode == EDIT_ROOM)
			{
				for (uint32 i = 0; i < numTiles; i++)
				{
					glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (pTile->GetID() >= TILE_NON_WALKABLE_INDEX)
						{
							pTile->SetMaterial(m_TileTints[(m_RoomBeingEdited - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
						}
					}
				}
			}
			else if (m_CurrentEditingMode == DELETE_ROOM)
			{
				for (uint32 i = 0; i < numTiles; i++)
				{
					glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (pTile->GetID() >= TILE_NON_WALKABLE_INDEX)
						{
							pTile->SetMaterial(MATERIAL::BLACK);
						}
					}
				}
			}
		}
	}
}

void Editor::OnMousePressed(MouseButton mousebutton, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->ContainsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI)
	{
		if (mousebutton == MouseButton::MOUSE_BUTTON_LEFT)
		{
			if (!m_Dragging)
			{
				if (m_CurrentEditingMode == ADD_ROOM)
				{
					m_Dragging = true;
					m_FirstCorner = CalculateGridPosition(position);
				}
				else if (m_CurrentEditingMode == EDIT_ROOM)
				{
					if (m_RoomBeingEdited == -1)
					{
						glm::ivec2 currentPos = CalculateGridPosition(position);

						if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
							currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
						{
							uint32 tileIndex = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos)->GetID();

							if (tileIndex >= TILE_NON_WALKABLE_INDEX)
							{
								m_RoomBeingEdited = tileIndex;
							}
						}
					}
					else
					{
						m_Dragging = true;
						m_FirstCorner = CalculateGridPosition(position);
					}
				}
				else if (m_CurrentEditingMode == DELETE_ROOM)
				{
					m_Dragging = true;
					m_FirstCorner = CalculateGridPosition(position);
				}
				else if (m_CurrentEditingMode == ADD_DOOR)
				{
					glm::ivec2 currentPos = CalculateGridPosition(position);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (!pTile->HasStairs())
						{
							pTile->SetDefaultMaterial(MATERIAL::WHITE);
							pTile->SetHasDoor(true);
						}
					}
				}
				else if (m_CurrentEditingMode == REMOVE_DOOR)
				{
					glm::ivec2 currentPos = CalculateGridPosition(position);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (pTile->HasDoor())
						{
							uint32 tileId = pTile->GetID();

							if (tileId >= TILE_SMALLEST_FREE)
							{
								pTile->SetDefaultMaterial(m_TileColors[(tileId - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
							}
							else if (tileId == TILE_NON_WALKABLE_INDEX)
							{
								pTile->SetDefaultMaterial(MATERIAL::BLACK);
							}
							else
							{
								pTile->SetDefaultMaterial(MATERIAL::WHITE);
							}

							pTile->SetHasDoor(false);
						}
					}
				}
				else if (m_CurrentEditingMode == ADD_STAIRS)
				{
					glm::ivec2 currentPos = CalculateGridPosition(position);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (!pTile->HasDoor())
						{
							pTile->SetDefaultMaterial(MATERIAL::BLUE);
							pTile->SetHasStairs(true);
						}
					}
				}
				else if (m_CurrentEditingMode == REMOVE_STAIRS)
				{
					glm::ivec2 currentPos = CalculateGridPosition(position);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);

						if (pTile->HasStairs())
						{
							uint32 tileId = pTile->GetID();

							if (tileId >= TILE_SMALLEST_FREE)
							{
								pTile->SetDefaultMaterial(m_TileColors[(tileId - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
							}
							else if (tileId == TILE_NON_WALKABLE_INDEX)
							{
								pTile->SetDefaultMaterial(MATERIAL::BLACK);
							}
							else
							{
								pTile->SetDefaultMaterial(MATERIAL::WHITE);
							}

							pTile->SetHasStairs(false);
						}
					}
				}
			}
		}
		else if (mousebutton == MouseButton::MOUSE_BUTTON_RIGHT)
		{
			if (m_CurrentEditingMode == EDIT_ROOM)
			{
				m_RoomBeingEdited = -1;
				m_Dragging = false;
			}
		}
	}

	/*if (gridPosition.x >= 0 && gridPosition.x <= m_pGrid->GetSize().x - 1 && gridPosition.y >= 0 && gridPosition.y <= m_pGrid->GetSize().y - 1)
	{
		m_pGrid->GetTile(gridPosition)->SetColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	}*/
}

void Editor::OnMouseReleased(MouseButton mousebutton, const glm::vec2& position)
{
	if (m_Dragging)
	{
		m_Dragging = false;
		glm::ivec2 secondCorner = CalculateGridPosition(position);
		glm::ivec2 lowestCorner = CalculateLowestCorner(m_FirstCorner, secondCorner);
		glm::ivec2 area = glm::abs(secondCorner - m_FirstCorner) + glm::ivec2(1, 1);
		uint32 numTiles = area.x * area.y;

		if (m_CurrentEditingMode == ADD_ROOM)
		{
			if (m_LargestIndexUsed < MAX_NUM_ROOMS + TILE_SMALLEST_FREE - 1)
			{
				m_LargestIndexUsed++;
				bool addedRoom = false;
				for (uint32 i = 0; i < numTiles; i++)
				{
					glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

					if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
						currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
					{
						Tile* pTile0 = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);
						Tile* pTile1 = m_ppGrids[m_CurrentGridIndex + 1]->GetTile(currentPos);

						if (pTile0->GetID() == TILE_NON_WALKABLE_INDEX)
						{
							pTile0->SetID(m_LargestIndexUsed);

							if (!pTile0->HasStairs() && !pTile0->HasDoor())
							{
								pTile0->SetDefaultMaterial(m_TileColors[(m_LargestIndexUsed - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
							}

							pTile1->SetID(m_LargestIndexUsed);
							pTile1->SetDefaultMaterial(m_TileColors[(m_LargestIndexUsed - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);

							addedRoom = true;
						}
					}
				}

				if (!addedRoom)
				{
					std::cout << "Did not add room at: " << m_LargestIndexUsed << std::endl;
					m_LargestIndexUsed--;
				}
				else
				{
					std::cout << "Added room with index: " << m_LargestIndexUsed << std::endl;
				}

			}
			else
			{
				std::cout << "MAX NUMBER OF ROOMS REACHED!" << std::endl;
			}

			m_FirstCorner = glm::ivec2(0);
		}
		else if (m_CurrentEditingMode == EDIT_ROOM)
		{
			for (uint32 i = 0; i < numTiles; i++)
			{
				glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

				if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
					currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
				{
					Tile* pTile0 = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);
					Tile* pTile1 = m_ppGrids[m_CurrentGridIndex + 1]->GetTile(currentPos);

					if (pTile0->GetID() >= TILE_NON_WALKABLE_INDEX)
					{
						pTile0->SetID(m_RoomBeingEdited);

						if (!pTile0->HasStairs() && !pTile0->HasDoor())
						{
							pTile0->SetDefaultMaterial(m_TileColors[(m_RoomBeingEdited - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
						}

						pTile1->SetID(m_RoomBeingEdited);
						pTile1->SetDefaultMaterial(m_TileColors[(m_RoomBeingEdited - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
					}
				}
			}

			NormalizeTileIndexes();
			m_FirstCorner = glm::ivec2(0);
		}
		else if (m_CurrentEditingMode == DELETE_ROOM)
		{
			for (uint32 i = 0; i < numTiles; i++)
			{
				glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

				if (currentPos.x >= 0 && currentPos.x <= m_ppGrids[m_CurrentGridIndex]->GetSize().x - 1 &&
					currentPos.y >= 0 && currentPos.y <= m_ppGrids[m_CurrentGridIndex]->GetSize().y - 1)
				{
					Tile* pTile0 = m_ppGrids[m_CurrentGridIndex]->GetTile(currentPos);
					Tile* pTile1 = m_ppGrids[m_CurrentGridIndex + 1]->GetTile(currentPos);

					if (pTile0->GetID() >= TILE_NON_WALKABLE_INDEX)
					{
						pTile0->SetID(TILE_NON_WALKABLE_INDEX);

						if (!pTile0->HasStairs() && !pTile0->HasDoor())
						{
							pTile0->SetDefaultMaterial(MATERIAL::BLACK);
						}

						pTile1->SetID(TILE_NON_WALKABLE_INDEX);
						pTile1->SetDefaultMaterial(MATERIAL::BLACK);
					}
				}
			}

			NormalizeTileIndexes();
			m_FirstCorner = glm::ivec2(0);
		}
	}
}

void Editor::OnKeyUp(KEY keycode)
{
	switch (keycode)
	{
		case KEY_LEFT_CTRL:
		{
			Input::SetCurserVisible(true);
			break;
		}
	}
}

void Editor::OnKeyDown(KEY keycode)
{
	switch (keycode)
	{
		case KEY_O:
		{
			using namespace std;
			Camera& camera = GetCurrentScene()->GetCamera();
			const glm::mat4& view = camera.GetViewMatrix();
			const glm::mat4& projection = camera.GetProjectionMatrix();
			const glm::mat4& combined = camera.GetCombinedMatrix();

			cout << "View: " << glm::to_string<glm::mat4>(view) << endl;
			cout << "Projection: " << glm::to_string<glm::mat4>(projection) << endl;
			cout << "Combined: " << glm::to_string<glm::mat4>(combined) << endl;

			break;
		}
		case KEY_LEFT_CTRL:
		{
			Input::SetCurserVisible(false);
			break;
		}
		case KEY_UP:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				GameObject* object = (GameObject*)button->GetUserData();
				object->SetPosition(object->GetPosition() + GetDirectionBasedOnCamera(FORWARD));
			}
			break;
		}
		case KEY_DOWN:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				GameObject* object = (GameObject*)button->GetUserData();
				object->SetPosition(object->GetPosition() + GetDirectionBasedOnCamera(BACKWARD));
			}
			break;
		}
		case KEY_LEFT:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				GameObject* object = (GameObject*)button->GetUserData();
				object->SetPosition(object->GetPosition() + GetDirectionBasedOnCamera(LEFT));
			}
			break;
		}
		case KEY_RIGHT:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				GameObject* object = (GameObject*)button->GetUserData();
				object->SetPosition(object->GetPosition() + GetDirectionBasedOnCamera(RIGHT));
			}
			break;
		}
		case KEY_SPACE:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				GameObject* object = (GameObject*)button->GetUserData();
				object->SetRotation(glm::vec4(0, 1, 0, object->GetRotation().w + glm::half_pi<float>()));
			}
			break;
		}
		case KEY_DELETE:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				if (button->GetUserData())
				{
					GameObject* object = (GameObject*)button->GetUserData();
					button->SetUserData(nullptr);
					std::vector<GameObject*>& objects = GetCurrentScene()->GetGameObjects();
					int32 index = 0;
					for (int i = 0; i < objects.size(); i++)
					{
						if (objects[i] == object)
						{
							index = i;
							objects.erase(objects.begin() + i);
							break;
						}
					}

					//Arange objects
					int counter = 0;
					for (int32 i = 0; i < m_pPanelScrollableEditMesh->GetChildren().size(); i++)
					{
						GUIObject* pObject = m_pPanelScrollableEditMesh->GetChildren()[i];
						if (object == pObject->GetUserData())
						{
							continue;
						}
						counter++;
						pObject->SetPosition(pObject->GetX(), m_pPanelScrollableEditMesh->GetClientHeight() - counter * ELEMENT_HEIGHT);
					}

					Delete(object);
					m_pPanelScrollableEditMesh->Remove(button);
				}	
			}
			break;
		}
		case KEY_ENTER:
		{
			ISelectable* selectable = m_SelectionHandlerMeshEdit.GetSelected();
			if (selectable)
			{
				Button* button = (Button*)selectable;
				GameObject* original = (GameObject*)button->GetUserData();
				int32 id = original->GetTypeId();
				GameObject* pGameObject = ResourceHandler::CreateGameObject(id);
				pGameObject->SetPosition(original->GetPosition());
				pGameObject->SetRotation(original->GetRotation());
				GetCurrentScene()->AddGameObject(pGameObject);

				CreateMesh(pGameObject, ResourceHandler::GetGameObjectName(id));
			}
			break;
		}
	}
}

void Editor::OnButtonReleased(Button* button)
{
	Editor* editor = GetEditor();
	if (button == editor->m_pButtonSave)
	{
		std::vector<glm::ivec3> stairs;
		std::vector<glm::ivec3> doors;
		WorldLevel** ppWorldLevels = editor->CreateWorldLevels(stairs, doors);
	

		int objects = 0;
		for (int i = 0; i < NUM_BOAT_LEVELS; i++)
		{
			std::vector<GameObject*> gameObjects;
			editor->GetGameObjects(gameObjects, i);
			objects += gameObjects.size();
		}

		std::cout << "Preparing to save: " << objects << " GameObjects" << std::endl;

		WorldObject* worldObjects = new WorldObject[objects];
		int counter = 0;
		for (int i = 0; i < NUM_BOAT_LEVELS; i++)
		{
			std::vector<GameObject*> gameObjects;
			editor->GetGameObjects(gameObjects, i);
			glm::ivec2 gridSize = editor->m_ppGrids[i]->GetSize() / 2;
			for (int j = 0; j < gameObjects.size(); j++)
			{
				GameObject* pGameObject = gameObjects[j];
				glm::uvec3 position = glm::uvec3(pGameObject->GetPosition()) + glm::uvec3(gridSize.x, i * 2, gridSize.y);
				int32 gameObject = pGameObject->GetTypeId();
				float rotation = pGameObject->GetRotation().w;

				if (gameObject == -1)
				{
					std::cout << "Skipping GameObject: " << gameObject << std::endl;
					continue;
				}
				worldObjects[counter++] = { position, static_cast<int32>(gameObject), rotation};
			}
		}

		World* pWorld = new World(ppWorldLevels, NUM_GRID_LEVELS, worldObjects, objects);
		uint32 numStairs = stairs.size();
		uint32 numDoors = doors.size();

		if (numStairs > 0)
		{
			pWorld->SetStairs(&stairs[0], numStairs);
		}

		if (numDoors > 0)
		{
			pWorld->SetDoors(&doors[0], numDoors);
		}

		WorldSerializer::Write("../Game/world.json", *pWorld);
		DeleteArr(ppWorldLevels);
		Delete(pWorld);
		DeleteArr(worldObjects);
	}
	else if (button == editor->m_pButtonLoad)
	{
		World* pWorld = WorldSerializer::Read("../Game/world.json");
		uint32 numWorldLevels = pWorld->GetNumLevels();

		assert(numWorldLevels == NUM_GRID_LEVELS);

		//Clean up previous scenes and cameras
		editor->ClearLevels();
		
		//Set up new scenes, grids and cameras
		Camera* pCameraPersp = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		pCameraPersp->CreatePerspective(glm::radians<float>(90.0f), editor->GetWindow().GetAspectRatio(), 0.01f, 100.0f);
		pCameraPersp->UpdateFromPitchYaw();

		Camera* pCameraOrth = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians<float>(-90.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		pCameraOrth->CreateOrthographic(30.0f * editor->GetWindow().GetAspectRatio() * editor->m_CameraZoom, 30.0f * editor->m_CameraZoom, 0.01f, 100.0f);
		pCameraOrth->UpdateFromPitchYaw();

		editor->m_ppScenes = new Scene*[NUM_GRID_LEVELS];
		editor->m_ppGrids = new Grid*[NUM_GRID_LEVELS];

		uint32 largestUsedTileId = 0;

		for (uint32 gridId = 0; gridId < NUM_GRID_LEVELS; gridId++)
		{
			//Create one scene for each grid level
			if (gridId % 2 == 0)
			{
				editor->m_ppScenes[gridId / 2] = new Scene();
				editor->m_ppScenes[gridId / 2]->SetCamera(pCameraPersp, 0);
				editor->m_ppScenes[gridId / 2]->SetCamera(pCameraOrth, 1);
			}

			//Create one grid for each grid level
			const WorldLevel* pWorldLevel = pWorld->GetLevel(gridId);
			const uint32* const * ppLevelIndexes = pWorldLevel->GetLevel();
			uint32 levelWidth = pWorldLevel->GetSizeX();
			uint32 levelHeight = pWorldLevel->GetSizeZ();
			int32 gridWidth = pWorldLevel->GetSizeX() - 2;
			int32 gridHeight = pWorldLevel->GetSizeZ() - 2;
			Grid* pGrid = new Grid(MATERIAL::BLACK, glm::ivec2(gridWidth, gridHeight), glm::vec3(-gridWidth / 2.0f, 0.0f, -gridHeight / 2.0f));

			for (uint32 x = 0; x < gridWidth; x++)
			{
				for (uint32 y = 0; y < gridHeight; y++)
				{
					Tile* pTile = pGrid->GetTile(glm::ivec2(x, y));
					uint32 tileId = ppLevelIndexes[x + 1][y + 1];

					if (tileId > largestUsedTileId)
					{
						largestUsedTileId = tileId;
					}

					if (tileId >= TILE_SMALLEST_FREE)
					{
						pTile->SetDefaultMaterial(editor->m_TileColors[(tileId - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
					}
					else if (tileId == TILE_NON_WALKABLE_INDEX)
					{
						pTile->SetDefaultMaterial(MATERIAL::BLACK);
					}
					else
					{
						pTile->SetDefaultMaterial(MATERIAL::WHITE);
					}

					pTile->SetID(tileId);
					editor->m_ppScenes[gridId / 2]->AddGameObject(pTile);
				}
			}

			editor->m_ppGrids[gridId] = pGrid;
		}

		const glm::ivec3* pStairs = pWorld->GetStairs();

		for (uint32 stairId = 0; stairId < pWorld->GetNumStairs(); stairId++)
		{
			const glm::ivec3& stairs = pStairs[stairId];
			Tile* pTile = editor->m_ppGrids[stairs.y]->GetTile(glm::ivec2(stairs.x - 1, stairs.z - 1));
			pTile->SetHasStairs(true);
			pTile->SetDefaultMaterial(MATERIAL::BLUE);
		}

		for (uint32 doorId = 0; doorId < pWorld->GetNumDoors(); doorId++)
		{
			const glm::ivec3& door = pWorld->GetDoor(doorId);
			Tile* pTile = editor->m_ppGrids[door.y]->GetTile(glm::ivec2(door.x - 1, door.z - 1));
			pTile->SetHasDoor(true);
			pTile->SetDefaultMaterial(MATERIAL::WHITE);
		}

		int gameObjects = pWorld->GetNumWorldObjects();

		for (int i = 0; i < gameObjects; i++)
		{
			WorldObject worldObject = pWorld->GetWorldObject(i);
			glm::ivec2 gridSize = editor->m_ppGrids[worldObject.TileId.y]->GetSize() / 2;
			int floorLevel = worldObject.TileId.y / 2;
			GameObject* pGameObject = ResourceHandler::CreateGameObject(worldObject.GameObject);
			pGameObject->SetPosition(editor->CalculateMeshPosition(glm::ivec3(static_cast<int32>(worldObject.TileId.x) - gridSize.x, worldObject.TileId.y % 2, static_cast<int32>(worldObject.TileId.z) - gridSize.y)));
			pGameObject->SetRotation(glm::vec4(0, 1, 0, worldObject.Rotation));
			editor->m_ppScenes[floorLevel]->AddGameObject(pGameObject);
			editor->CreateMesh(pGameObject, ResourceHandler::GetGameObjectName(worldObject.GameObject), i, gameObjects);
		}

		editor->m_RoomBeingEdited = -1;
		editor->m_LargestIndexUsed = largestUsedTileId;
		editor->m_MouseMaterial = MATERIAL::WHITE;

		Delete(pWorld);
	}
	else if (button == editor->m_pButtonRoom)
	{
		editor->m_pPanelEditor->SetVisible(true);
		editor->m_pPanelMesh->SetVisible(false);
		for (int i = 0; i < NUM_BOAT_LEVELS; i++)
		{
			editor->m_ppScenes[i]->SelectCamera(1);
		}
	}
	else if (button == editor->m_pButtonMesh)
	{
		editor->m_pPanelMesh->SetVisible(true);
		editor->m_pPanelEditor->SetVisible(false);
		for (int i = 0; i < NUM_BOAT_LEVELS; i++)
		{
			editor->m_ppScenes[i]->SelectCamera(0);
		}

		editor->CreateWalls();
	}
	else
	{
		uint32 id = reinterpret_cast<uint32>(button->GetUserData());
		std::cout << "Creting GameObject from ID " << id << std::endl;

		GameObject* pGameObject = ResourceHandler::CreateGameObject(id);
		pGameObject->SetPosition(editor->CalculateMeshPosition(glm::vec3(0, 0, 0)));
		editor->GetCurrentScene()->AddGameObject(pGameObject);

		editor->CreateMesh(pGameObject, ResourceHandler::GetGameObjectName(id));
	}
}

Editor* Editor::GetEditor()
{
	return (Editor*)&GetInstance();
}

void Editor::OnUpdate(float dtS)
{
	static float tempRotation = 0.0f;
	tempRotation += 1.0f * dtS;

	static float cameraSpeed = 5.0f;
	static float cameraAngularSpeed = 1.5f;
	static float cameraZoomSpeed = 0.2f;

	if (m_pPanelEditor->IsVisible())
	{
		if (Input::IsKeyDown(KEY_W))
		{
			GetCurrentScene()->GetCamera().MoveLocalCoords(glm::vec3(0.0f, cameraSpeed * dtS, 0.0f));
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			GetCurrentScene()->GetCamera().MoveLocalCoords(glm::vec3(0.0f, -cameraSpeed * dtS, 0.0f));
		}

		if (Input::IsKeyDown(KEY_A))
		{
			GetCurrentScene()->GetCamera().MoveLocalCoords(glm::vec3(cameraSpeed * dtS, 0.0f, 0.0f));
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			GetCurrentScene()->GetCamera().MoveLocalCoords(glm::vec3(-cameraSpeed * dtS, 0.0f, 0.0f));
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_CameraZoom -= cameraZoomSpeed * dtS;
			m_CameraZoom = glm::max(m_CameraZoom, 0.01f);
			Camera& camera = GetCurrentScene()->GetCamera();
			camera.CreateOrthographic(30.0f * GetWindow().GetAspectRatio() * m_CameraZoom, 30.0f * m_CameraZoom, 0.01f, 100.0f);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_CameraZoom += cameraZoomSpeed * dtS;
			Camera& camera = GetCurrentScene()->GetCamera();
			camera.CreateOrthographic(30.0f * GetWindow().GetAspectRatio() * m_CameraZoom, 30.0f * m_CameraZoom, 0.01f, 100.0f);
		}
		GetCurrentScene()->GetCamera().UpdateFromPitchYaw();
	}
	else
	{
		GetCurrentScene()->GetCamera().UpdateFromLookAt();
	}
	GetCurrentScene()->OnUpdate(dtS);

	//GetCurrentScene()->GetCamera().UpdateFromPitchYaw();
}

void Editor::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*GetCurrentScene(), nullptr, dtS);
}