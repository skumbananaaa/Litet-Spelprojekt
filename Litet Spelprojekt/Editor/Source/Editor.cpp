#include <Graphics/Renderers/DefferedRenderer.h>
#include "..\Include\Editor.h"

Slider* m_pSlider2;

void OnButtonPressed(Button* button)
{
	std::cout << "Button Pressed" << std::endl;
}

void OnButtonReleased(Button* button)
{
	std::cout << "Button Released" << std::endl;
	m_pSlider2->SetVisible(!m_pSlider2->IsVisible());
}

void OnSliderChanged(Slider* slider, float percentage)
{
	std::cout << "Slider: "  << percentage << std::endl;
}

Editor::Editor() noexcept : Application(false)
{
	m_pRenderer = new OrthographicRenderer();
	std::cout << "Editor" << std::endl;

	Shader vShader;
	Shader fShader;

	vShader.CompileFromFile("Resources/Shaders/VShader.glsl", VERTEX_SHADER);
	fShader.CompileFromFile("Resources/Shaders/FShader.glsl", FRAGMENT_SHADER);

	//m_pShaderProgramDefault = new ShaderProgram(vShader, fShader);

	std::vector<std::string> data;
	data.push_back("Line 1");
	data.push_back("Line 2");
	data.push_back("Line 3");
	data.push_back("Line 4");

	//WorldSerializer::Write("test.txt", data);


	const int WIDTH = 10;
	const int HEIGHT = 10;
	const int DEPTH = 10;

	glm::mat4 transform(1.0f);
	this->m_pGridUniform = new UniformBuffer(glm::value_ptr(transform), 1, sizeof(glm::mat4));

	//this->m_pGridMesh = Mesh::CreateGrid(WIDTH, HEIGHT, DEPTH);

	m_pScene = new Scene();

	Camera* pCamera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians<float>(-90.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	//pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.01f, 100.0f);
	pCamera->CreateOrthographic(30.0f * aspect, 30.0f, 0.01f, 100.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

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
	m_pButtonFloor1 = new Button(10, m_pPanelFloor->GetHeight() - 100, 140, 50, "Floor 1", nullptr, OnButtonReleased);
	m_pButtonFloor2 = new Button(10, m_pPanelFloor->GetHeight() - 160, 140, 50, "Floor 2", nullptr, OnButtonReleased);
	m_pButtonFloor3 = new Button(10, m_pPanelFloor->GetHeight() - 220, 140, 50, "Floor 3", nullptr, OnButtonReleased);
	m_pPanelFloor->Add(m_pTextViewFloor);
	m_pPanelFloor->Add(m_pButtonFloor1);
	m_pPanelFloor->Add(m_pButtonFloor2);
	m_pPanelFloor->Add(m_pButtonFloor3);
	m_pPanelFloor->SetDeleteAllChildrenOnDestruction(true);

	m_pPanelEditor = new Panel(GetWindow().GetWidth() - 160, GetWindow().GetHeight() / 4, 160, GetWindow().GetHeight() / 2);
	m_pTextViewEditor = new TextView(0, m_pPanelEditor->GetHeight() - 50, m_pPanelEditor->GetWidth(), 50, "Select Tool", TextAlignment::CENTER);
	m_pButtonAdd = new Button(10, m_pPanelEditor->GetHeight() - 100, 140, 50, "New room", nullptr, OnButtonReleased);
	m_pButtonEdit = new Button(10, m_pPanelEditor->GetHeight() - 160, 140, 50, "Edit room", nullptr, OnButtonReleased);
	m_pButtonRemove = new Button(10, m_pPanelEditor->GetHeight() - 220, 140, 50, "Delete room", nullptr, OnButtonReleased);
	m_pPanelEditor->Add(m_pTextViewEditor);
	m_pPanelEditor->Add(m_pButtonAdd);
	m_pPanelEditor->Add(m_pButtonEdit);
	m_pPanelEditor->Add(m_pButtonRemove);
	m_pPanelEditor->SetDeleteAllChildrenOnDestruction(true);

	GetGUIManager().Add(m_pPanelTop);
	GetGUIManager().Add(m_pPanelFloor);
	GetGUIManager().Add(m_pPanelEditor);

	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec3(0.0f, 0.5f, 0.5f));
	m_pScene->AddDirectionalLight(pDirectionalLight);

	m_pGrid = new Grid(glm::ivec2(20, 20), glm::vec3(-10.0f, 0.0f, -10.0f));

	int temp_map[20][20]{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{1, 3, 3, 3, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
		{1, 3, 2, 2, 2, 2, 0, 3, 3, 3, 5, 5, 2, 2, 2, 2, 0, 4, 4, 1 },
		{1, 3, 2, 2, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
		{1, 3, 2, 2, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
		{1, 3, 2, 0, 2, 2, 2, 3, 0, 3, 5, 5, 2, 2, 2, 0, 2, 4, 0, 1 },
		{1, 3, 3, 3, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 2, 5, 5, 3, 3, 1 },
		{1, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 3, 3, 1 },
		{1, 3, 3, 3, 3, 3, 4, 4, 4, 4, 0, 5, 5, 5, 5, 5, 5, 3, 3, 1 },
		{1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 3, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 1, 4, 4, 3, 2, 2, 2, 2, 2, 2, 0, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 2, 2, 1 },
		{1, 3, 2, 2, 2, 2, 0, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 3, 3, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 3, 3, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 3, 3, 1 },
		{1, 3, 2, 2, 2, 0, 2, 5, 0, 5, 3, 3, 2, 2, 2, 0, 2, 4, 0, 1 },
		{1, 3, 3, 3, 2, 3, 5, 5, 5, 5, 3, 3, 3, 3, 2, 3, 4, 4, 4, 1 },
		{1, 3, 3, 3, 3, 3, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 4, 4, 4, 1 },
		{1, 3, 3, 3, 3, 3, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 4, 4, 4, 1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	for (int i = 0; i < m_pGrid->GetSize().x; i++)
	{
		for (int j = 0; j < m_pGrid->GetSize().y; j++)
		{
			m_pGrid->GetTile(glm::ivec2(i, j))->SetID(temp_map[i][j]);
			m_pGrid->SetColor(glm::ivec2(i, j), glm::vec4(temp_map[i][j] / 10.0f, temp_map[i][j] / 10.0f, temp_map[i][j] / 10.0f, 1.0f));
			GameObject* g = m_pGrid->GetTile(glm::ivec2(i, j));
			m_pScene->AddGameObject(g);
		}
	}

	//GetGraphicsContext().Disable(Cap::CULL_FACE);
}

Editor::~Editor()
{
	Delete(m_pRenderer);
	Delete(m_pGrid);
	Delete(m_pScene);

	Delete(m_pPanelTop);
	Delete(m_pPanelFloor);
	Delete(m_pPanelEditor);
}

void Editor::OnMousePressed(MouseButton mousebutton, const glm::vec2& position)
{
	glm::vec2 clipSpacePosition(position.x / static_cast<float>(GetWindow().GetWidth()), position.y / static_cast<float>(GetWindow().GetHeight()));
	clipSpacePosition = (clipSpacePosition - glm::vec2(0.5f)) * 2.0f;
	glm::vec3 worldPosition = m_pScene->GetCamera().GetInverseCombinedMatrix() * glm::vec4(0.0f, clipSpacePosition.y, clipSpacePosition.x, 1.0f);
	glm::uvec2 gridPosition(static_cast<uint32>(worldPosition.z) + 10, static_cast<uint32>(worldPosition.y) + 10);
	gridPosition.x = glm::clamp<uint32>(gridPosition.x, 0, m_pGrid->GetSize().x - 1);
	gridPosition.y = glm::clamp<uint32>(gridPosition.y, 0, m_pGrid->GetSize().y - 1);

	Tile* tile = m_pGrid->GetTile(gridPosition);

	//Material* material = new Material(tile->GetMaterial());
	tile->SetColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	//material->SetColor();
	//tile->SetMaterial(material);
	std::cout << glm::to_string(clipSpacePosition) << std::endl;
	std::cout << glm::to_string(worldPosition) << std::endl;
}

void Editor::OnMouseReleased(MouseButton mousebutton, const glm::vec2& position)
{
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

	}
	else if (button == editor->m_pButtonLoad)
	{

	}
	else if (button == editor->m_pButtonRoom)
	{
		editor->m_pButtonAdd->SetText("New Room");
		editor->m_pButtonEdit->SetText("Edit Room");
		editor->m_pButtonRemove->SetText("Delete Room");
	}
	else if (button == editor->m_pButtonMesh)
	{
		editor->m_pButtonAdd->SetText("New Mesh");
		editor->m_pButtonEdit->SetText("Edit Mesh");
		editor->m_pButtonRemove->SetText("Delete Mesh");
	}
	else if (button == editor->m_pButtonFloor1)
	{

	}
	else if (button == editor->m_pButtonFloor2)
	{

	}
	else if (button == editor->m_pButtonFloor3)
	{

	}
	else if (button == editor->m_pButtonAdd)
	{

	}
	else if (button == editor->m_pButtonEdit)
	{

	}
	else if (button == editor->m_pButtonRemove)
	{

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

	/*m_pScene->GetCamera().UpdateFromPitchYaw();
	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_pPerFrameUniform->UpdateData(&m_PerFrameArray);*/
}

void Editor::OnRender(float dtS)
{
	/*GetGraphicsContext().SetProgram(m_pShaderProgramDefault);
	GetGraphicsContext().SetUniformBuffer(m_pPerFrameUniform, 1);

	GetGraphicsContext().SetUniformBuffer(m_pGridUniform, 0);
	GetGraphicsContext().DrawMesh(*m_pGridMesh, PT_LINES);*/
	m_pRenderer->DrawScene(*m_pScene, dtS);
}
