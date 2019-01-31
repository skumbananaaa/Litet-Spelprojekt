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

	Camera* pCamera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians<float>(-90.0f), 0.0f);
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	//pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.01f, 100.0f);
	pCamera->CreateOrthographic(30.0f, 30.0f, 0.01f, 100.0f);
	pCamera->UpdateFromPitchYaw();
	//pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	//m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	//m_pPerFrameUniform = new UniformBuffer(m_PerFrameArray, 1, sizeof(m_PerFrameArray));
	

	Texture2D* b = new Texture2D("Resources/Textures/test4.png", TEX_FORMAT::TEX_FORMAT_RGBA, false);
	Texture2D* f = new Texture2D("Resources/Textures/test5.png", TEX_FORMAT::TEX_FORMAT_RGBA, false);

	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	m_pPanel = new Panel(0, 50, 500, 500, new Texture2D("Resources/Textures/test3.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));

	m_pButton = new Button(25, 400, 200, 50, "Button 1", OnButtonPressed, OnButtonReleased);
	m_pButton->SetTexture(new Texture2D("Resources/Textures/test.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));
	m_pButton->SetOnPressedTexture(new Texture2D("Resources/Textures/test2.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));

	m_pButton2 = new Button(350, 400, 200, 50, "Button 2", OnButtonPressed, OnButtonReleased);
	m_pButton2->SetTexture(new Texture2D("Resources/Textures/test.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));
	m_pButton2->SetOnPressedTexture(new Texture2D("Resources/Textures/test2.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));

	m_pSlider = new Slider(10, 100, 400, 30, b, f, OnSliderChanged);
	m_pSlider->SetRatio(0.5F);
	m_pSlider->SetPercentage(0.0F);

	m_pSlider2 = new Slider(450, 50, 30, 400, b, f, OnSliderChanged);
	m_pSlider2->SetRatio(0.5F);
	m_pSlider2->SetPercentage(0.0F);


	m_pPanelScrollable = new PanelScrollable(520, 50, 500, 500, 500, 1000, new Texture2D("Resources/Textures/test6.png", TEX_FORMAT::TEX_FORMAT_RGBA, false), b, f);

	m_pPanelScrollable->Add(m_pButton);
	m_pPanelScrollable->Add(m_pButton2);
	m_pPanelScrollable->Add(m_pSlider);
	m_pPanelScrollable->Add(m_pSlider2);

	/*GetGUIManager().Add(m_pPanel);
	GetGUIManager().Add(m_pTextViewFPS);
	GetGUIManager().Add(m_pTextViewUPS);
	GetGUIManager().Add(m_pPanelScrollable);*/

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
	//Delete(m_pShaderProgramDefault);
	Delete(m_pRenderer);
	Delete(m_pGrid);
	Delete(m_pScene);
	Delete(m_pTextViewFPS);
	Delete(m_pTextViewUPS);
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
	

	m_pTextViewFPS->SetText("FPS " + std::to_string(GetFPS()));
	m_pTextViewUPS->SetText("UPS " + std::to_string(GetUPS()));
}

void Editor::OnRender(float dtS)
{
	/*GetGraphicsContext().SetProgram(m_pShaderProgramDefault);
	GetGraphicsContext().SetUniformBuffer(m_pPerFrameUniform, 1);

	GetGraphicsContext().SetUniformBuffer(m_pGridUniform, 0);
	GetGraphicsContext().DrawMesh(*m_pGridMesh, PT_LINES);*/
	m_pRenderer->DrawScene(*m_pScene, dtS);
}
