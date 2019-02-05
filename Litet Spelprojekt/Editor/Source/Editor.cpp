#include <Graphics/Renderers/DefferedRenderer.h>
#include "..\Include\Editor.h"

Editor::Editor() noexcept : Application(false),
	m_SelectionHandlerFloor(true),
	m_SelectionHandlerRoom(false)
{
	ResourceHandler::LoadResources(this);

	m_pRenderer = new OrthographicRenderer();
	std::cout << "Editor" << std::endl;

	m_pScene = new Scene();


	Camera* pCamera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians<float>(-90.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreateOrthographic(30.0f * aspect, 30.0f, 0.01f, 100.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	/*Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.01f, 100.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);*/

	const int32 gridWidth = 40;
	const int32 gridHeight = 20;
	m_pGrid = new Grid(MATERIAL::BLACK, glm::ivec2(gridHeight, gridWidth), glm::vec3(-gridHeight / 2.0f, 0.0f, -gridWidth / 2.0f));

	for (int i = 0; i < m_pGrid->GetSize().x; i++)
	{
		for (int j = 0; j < m_pGrid->GetSize().y; j++)
		{
			Tile* tile = m_pGrid->GetTile(glm::ivec2(i, j));
			tile->SetID(TILE_NON_WALKABLE_INDEX);
			m_pScene->AddGameObject(tile);
		}
	}

	m_RoomBeingEdited = -1;
	m_LargestIndexUsed = TILE_SMALLEST_FREE - 1;
	m_MouseMaterial = MATERIAL::WHITE;

	m_Dragging = false;
	m_CurrentEditingMode = NONE;
	//GetGraphicsContext().Disable(Cap::CULL_FACE);

}

Editor::~Editor()
{
	m_SelectionHandlerFloor.Release();
	m_SelectionHandlerRoom.Release();

	Delete(m_pRenderer);
	Delete(m_pGrid);
	Delete(m_pScene);

	Delete(m_pPanelTop);
	Delete(m_pPanelFloor);
	Delete(m_pPanelEditor);

	ResourceHandler::ReleaseResources();
}

void Editor::OnResourcesLoaded()
{
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


	m_pPanelFloor = new Panel(0, GetWindow().GetHeight() / 4, 160, GetWindow().GetHeight() / 2);
	m_pTextViewFloor = new TextView(0, m_pPanelFloor->GetHeight() - 50, m_pPanelFloor->GetWidth(), 50, "Select Floor", TextAlignment::CENTER);
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


	m_pPanelEditor = new Panel(GetWindow().GetWidth() - 160, GetWindow().GetHeight() / 4, 160, GetWindow().GetHeight() / 2);
	m_pTextViewEditor = new TextView(0, m_pPanelEditor->GetHeight() - 50, m_pPanelEditor->GetWidth(), 50, "Room Tool", TextAlignment::CENTER);
	m_pButtonAddRoom = new Button(10, m_pPanelEditor->GetHeight() - 100, 140, 50, "New Room");
	m_pButtonEditRoom = new Button(10, m_pPanelEditor->GetHeight() - 160, 140, 50, "Edit Room");
	m_pButtonRemoveRoom = new Button(10, m_pPanelEditor->GetHeight() - 220, 140, 50, "Delete Room");
	m_pButtonAddDoor = new Button(10, m_pPanelEditor->GetHeight() - 280, 140, 50, "Add Door");
	m_pButtonRemoveDoor = new Button(10, m_pPanelEditor->GetHeight() - 340, 140, 50, "Remove Door");
	m_pButtonAddRoom->SetUserData(reinterpret_cast<void*>(ADD_ROOM));
	m_pButtonEditRoom->SetUserData(reinterpret_cast<void*>(EDIT_ROOM));
	m_pButtonRemoveRoom->SetUserData(reinterpret_cast<void*>(DELETE_ROOM));
	m_pButtonAddDoor->SetUserData(reinterpret_cast<void*>(ADD_DOOR));
	m_pButtonRemoveDoor->SetUserData(reinterpret_cast<void*>(REMOVE_DOOR));
	m_pPanelEditor->Add(m_pTextViewEditor);
	m_pPanelEditor->Add(m_pButtonAddRoom);
	m_pPanelEditor->Add(m_pButtonEditRoom);
	m_pPanelEditor->Add(m_pButtonRemoveRoom);
	m_pPanelEditor->Add(m_pButtonAddDoor);
	m_pPanelEditor->Add(m_pButtonRemoveDoor);
	m_pPanelEditor->SetDeleteAllChildrenOnDestruction(true);
	m_SelectionHandlerRoom.AddSelectionListener(this);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonAddRoom);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonEditRoom);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonRemoveRoom);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonAddDoor);
	m_SelectionHandlerRoom.AddSelectable(m_pButtonRemoveDoor);


	m_pPanelMesh = new Panel(GetWindow().GetWidth() - 160, GetWindow().GetHeight() / 4, 160, GetWindow().GetHeight() / 2);
	m_pTextViewMesh = new TextView(0, m_pPanelEditor->GetHeight() - 50, m_pPanelEditor->GetWidth(), 50, "Mesh Tool", TextAlignment::CENTER);
	m_pPanelScrollableMesh = new PanelScrollable(10, 10, m_pPanelMesh->GetWidth() - 20, m_pPanelMesh->GetHeight() - m_pTextViewMesh->GetHeight() - 10, m_pPanelMesh->GetWidth(), m_pPanelMesh->GetHeight() * 2);
	m_pPanelMesh->Add(m_pTextViewMesh);
	m_pPanelMesh->Add(m_pPanelScrollableMesh);
	m_pPanelMesh->SetVisible(false);
	m_pPanelMesh->SetDeleteAllChildrenOnDestruction(true);

	GetGUIManager().Add(m_pPanelTop);
	GetGUIManager().Add(m_pPanelFloor);
	GetGUIManager().Add(m_pPanelEditor);
	GetGUIManager().Add(m_pPanelMesh);

	std::vector<MESH_DESC> meshDescs;
	ResourceHandler::QuaryMeshes(meshDescs);
	float buttonHeight = 50;
	m_pPanelScrollableMesh->SetClientSize(m_pPanelScrollableMesh->GetWidth(), buttonHeight * meshDescs.size());

	for (int64 i = 0; i < meshDescs.size(); i++)
	{
		MESH_DESC meshDesc = meshDescs[i];
		Button* button = new Button(0, m_pPanelScrollableMesh->GetClientHeight() - (i + 1) * buttonHeight, m_pPanelScrollableMesh->GetClientWidth(), buttonHeight, meshDesc.name, nullptr, OnButtonReleased);
		button->SetTextAlignment(TextAlignment::CENTER_VERTICAL);
		button->SetUserData(reinterpret_cast<void*>(meshDesc.mesh));
		m_pPanelScrollableMesh->Add(button);
	}
	float hDelta = 360.0f / (float)MAX_NUM_ROOMS;
	float currentH = 0.0f;
	for (uint32 i = 0; i < MAX_NUM_ROOMS; i++)
	{
		m_TileColors[i] = ResourceHandler::RegisterMaterial(HSVA2RGBA(glm::vec4(currentH, 1.0f, 1.0f, 1.0f)));
		m_TileTints[i] = ResourceHandler::RegisterMaterial(HSVA2RGBA(glm::vec4(currentH, 1.0f, 0.5f, 1.0f)));
		currentH += hDelta;
	}

	std::cout << "Resources Loaded!" << std::endl;
}

void Editor::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{
	if (handler == &m_SelectionHandlerFloor)
	{
		Button* button = (Button*)selection;
		uint32 floorIndex = reinterpret_cast<uint32>(button->GetUserData());
		std::cout << "New Floor Index: " << floorIndex << std::endl;
	}
	else if (handler == &m_SelectionHandlerRoom)
	{
		Button* button = (Button*)selection;
		EditingMode newMode = (EditingMode)reinterpret_cast<uint32>(button->GetUserData());
		std::cout << "New Editing Mode: " << newMode << std::endl;

		m_CurrentEditingMode = newMode;
		m_Dragging = false;
		m_RoomBeingEdited = -1;
		m_MouseMaterial = MATERIAL::WHITE;
	}
}

void Editor::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{
	if (handler == &m_SelectionHandlerFloor)
	{
		Button* button = (Button*)selection;
		uint32 floorIndex = reinterpret_cast<uint32>(button->GetUserData());
		std::cout << "Last Floor Index: " << floorIndex << std::endl;
	}
	else if (handler == &m_SelectionHandlerRoom)
	{
		Button* button = (Button*)selection;
		EditingMode newMode = (EditingMode)reinterpret_cast<uint32>(button->GetUserData());
		std::cout << "Last Editing Mode: " << newMode << std::endl;
		m_CurrentEditingMode = NONE;
	}
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

	for (uint32 x = 0; x < m_pGrid->GetSize().x; x++)
	{
		for (uint32 y = 0; y < m_pGrid->GetSize().y; y++)
		{
			Tile* tile = m_pGrid->GetTile(glm::ivec2(x, y));
			int32 index = tile->GetID() - TILE_SMALLEST_FREE;

			if (index >= 0)
			{
				indexesExisting[index] = true;
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

	for (uint32 x = 0; x < m_pGrid->GetSize().x; x++)
	{
		for (uint32 y = 0; y < m_pGrid->GetSize().y; y++)
		{
			Tile* tile = m_pGrid->GetTile(glm::ivec2(x, y));
			int32 index = tile->GetID() - TILE_SMALLEST_FREE;

			if (index >= 0)
			{
				uint32 newIndex = tile->GetID() - indexesMissingBefore[index];
				tile->SetID(newIndex);
				tile->SetDefaultMaterial(m_TileColors[(newIndex - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
			}
		}
	}
}

glm::ivec2 Editor::CalculateGridPosition(const glm::vec2& mousePosition) noexcept
{
	glm::vec2 clipSpacePosition(mousePosition.x / static_cast<float>(GetWindow().GetWidth()), mousePosition.y / static_cast<float>(GetWindow().GetHeight()));
	clipSpacePosition = (clipSpacePosition - glm::vec2(0.5f)) * 2.0f;
	glm::vec3 worldPosition = m_pScene->GetCamera().GetInverseCombinedMatrix() * glm::vec4(clipSpacePosition.x, clipSpacePosition.y, 0.0f, 1.0f);
	glm::ivec2 gridPosition(
		static_cast<uint32>(glm::round(worldPosition.x)) + m_pGrid->GetSize().x / 2,
		static_cast<uint32>(glm::round(worldPosition.z)) + m_pGrid->GetSize().y / 2);
	return gridPosition;
}

glm::ivec2 Editor::CalculateLowestCorner(const glm::ivec2& firstCorner, const glm::ivec2& secondCorner) noexcept
{
	glm::ivec2 lowestCorner;
	lowestCorner.x = glm::min(firstCorner.x, secondCorner.x);
	lowestCorner.y = glm::min(firstCorner.y, secondCorner.y);
	return lowestCorner;
}

void Editor::OnMouseMove(const glm::vec2& position)
{
	for (uint32 x = 0; x < m_pGrid->GetSize().x; x++)
	{
		for (uint32 y = 0; y < m_pGrid->GetSize().y; y++)
		{
			m_pGrid->GetTile(glm::ivec2(x, y))->ResetMaterial();
		}
	}

	glm::ivec2 gridPos = CalculateGridPosition(position);
	if (gridPos.x >= 0 && gridPos.x <= m_pGrid->GetSize().x - 1 && gridPos.y >= 0 && gridPos.y <= m_pGrid->GetSize().y - 1)
	{
		m_pGrid->GetTile(gridPos)->SetMaterial(m_MouseMaterial);
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

				if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
				{
					Tile* tile = m_pGrid->GetTile(currentPos);

					if (tile->GetID() == TILE_NON_WALKABLE_INDEX)
					{
						tile->SetMaterial(m_TileTints[(m_LargestIndexUsed - TILE_SMALLEST_FREE + 1) % MAX_NUM_ROOMS]);
					}
				}
			}
		}
		else if (m_CurrentEditingMode == EDIT_ROOM)
		{
			for (uint32 i = 0; i < numTiles; i++)
			{
				glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

				if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
				{
					Tile* tile = m_pGrid->GetTile(currentPos);

					if (tile->GetID() >= TILE_NON_WALKABLE_INDEX)
					{
						tile->SetMaterial(m_TileTints[(m_RoomBeingEdited - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
					}
				}
			}
		}
		else if (m_CurrentEditingMode == DELETE_ROOM)
		{
			for (uint32 i = 0; i < numTiles; i++)
			{
				glm::ivec2 currentPos = lowestCorner + glm::ivec2(i % area.x, i / area.x);

				if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
				{
					Tile* tile = m_pGrid->GetTile(currentPos);

					if (tile->GetID() >= TILE_NON_WALKABLE_INDEX)
					{
						tile->SetMaterial(MATERIAL::BLACK);
					}
				}
			}
		}
	}
}

void Editor::OnMousePressed(MouseButton mousebutton, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* object : GetGUIManager().GetChildren())
	{
		if (object->ContainsPoint(position))
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

						if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
						{
							uint32 tileIndex = m_pGrid->GetTile(currentPos)->GetID();

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
					if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
					{
						Tile* tile = m_pGrid->GetTile(currentPos);
						tile->SetID(TILE_DOOR_INDEX);
						tile->SetDefaultMaterial(MATERIAL::WHITE);
					}
				}
				else if (m_CurrentEditingMode == REMOVE_DOOR)
				{
					glm::ivec2 currentPos = CalculateGridPosition(position);
					if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
					{
						Tile* tile = m_pGrid->GetTile(currentPos);

						if (tile->GetID() == TILE_DOOR_INDEX)
						{
							tile->SetID(TILE_NON_WALKABLE_INDEX);
							tile->SetDefaultMaterial(MATERIAL::BLACK);
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

					if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
					{
						Tile* tile = m_pGrid->GetTile(currentPos);

						if (tile->GetID() == TILE_NON_WALKABLE_INDEX)
						{
							tile->SetID(m_LargestIndexUsed);
							tile->SetDefaultMaterial(m_TileColors[(m_LargestIndexUsed - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
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

				if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
				{
					Tile* tile = m_pGrid->GetTile(currentPos);

					if (tile->GetID() >= TILE_NON_WALKABLE_INDEX)
					{
						tile->SetID(m_RoomBeingEdited);
						tile->SetDefaultMaterial(m_TileColors[(m_RoomBeingEdited - TILE_SMALLEST_FREE) % MAX_NUM_ROOMS]);
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

				if (currentPos.x >= 0 && currentPos.x <= m_pGrid->GetSize().x - 1 && currentPos.y >= 0 && currentPos.y <= m_pGrid->GetSize().y - 1)
				{
					Tile* tile = m_pGrid->GetTile(currentPos);

					if (tile->GetID() >= TILE_NON_WALKABLE_INDEX)
					{
						tile->SetID(TILE_NON_WALKABLE_INDEX);
						tile->SetDefaultMaterial(MATERIAL::BLACK);
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
}

void Editor::OnKeyDown(KEY keycode)
{
	switch (keycode)
	{
		case KEY_O:
		{
			using namespace std;
			Camera& camera = m_pScene->GetCamera();
			const glm::mat4& view = camera.GetViewMatrix();
			const glm::mat4& projection = camera.GetProjectionMatrix();
			const glm::mat4& combined = camera.GetCombinedMatrix();

			cout << "View: " << glm::to_string<glm::mat4>(view) << endl;
			cout << "Projection: " << glm::to_string<glm::mat4>(projection) << endl;
			cout << "Combined: " << glm::to_string<glm::mat4>(combined) << endl;

			break;
		}
	}
}

void Editor::OnButtonReleased(Button* button)
{
	Editor* editor = GetEditor();
	if (button == editor->m_pButtonSave)
	{
		uint32 level0SizeX = editor->m_pGrid->GetSize().x;
		uint32 level0SizeY = editor->m_pGrid->GetSize().y;
		uint32* level0 = new uint32[level0SizeX * level0SizeY];

		for (uint32 x = 0; x < level0SizeX; x++)
		{
			for (uint32 y = 0; y < level0SizeY; y++)
			{
				level0[x * level0SizeY + y] = editor->m_pGrid->GetTile(glm::ivec2(x, y))->GetID();
			}
		}

		WorldLevel* worldLevel0 = new WorldLevel(level0, level0SizeX, level0SizeY);

		WorldLevel* worldLevels[1] =
		{
			worldLevel0
		};

		WorldObject worldObjects[5] =
		{
			{ glm::uvec3(0, 1, 2), 1337, 69 },
			{ glm::uvec3(3, 4, 5), 1337, 420 },
			{ glm::uvec3(6, 7, 8), 1337, 5 },
			{ glm::uvec3(9, 10, 11), 1337, 15 },
			{ glm::uvec3(12, 13, 14), 1337, 8 },
		};

		World* world = new World(worldLevels, 1, worldObjects, 5);
		WorldSerializer::Write("test.json", *world);
		Delete(world);
	}
	else if (button == editor->m_pButtonLoad)
	{

	}
	else if (button == editor->m_pButtonRoom)
	{
		editor->m_pPanelEditor->SetVisible(true);
		editor->m_pPanelMesh->SetVisible(false);
	}
	else if (button == editor->m_pButtonMesh)
	{
		editor->m_pPanelMesh->SetVisible(true);
		editor->m_pPanelEditor->SetVisible(false);
	}
	else
	{
		uint32 id = reinterpret_cast<uint32>(button->GetUserData());
		std::cout << "Creting Mesh from ID " << id << std::endl;

		GameObject* gameObject = new GameObject();
		gameObject->SetMaterial(MATERIAL::WHITE);
		gameObject->SetMesh(id);
		gameObject->SetPosition(glm::vec3(0, 0, 0));
		editor->m_pScene->AddGameObject(gameObject);
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
	static float angularSpeed = 1.5f;

	if (Input::IsKeyDown(KEY_W))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Up, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_S))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Down, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_A))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Left, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_D))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Right, cameraSpeed * dtS);
	}

	m_pScene->GetCamera().UpdateFromPitchYaw();
}

void Editor::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pScene, dtS);
}