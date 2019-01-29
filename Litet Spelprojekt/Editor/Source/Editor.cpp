#include "..\Include\Editor.h"

void OnButtonPressed(Button* button)
{
	std::cout << "Button Pressed" << std::endl;
}

void OnButtonReleased(Button* button)
{
	std::cout << "Button Released" << std::endl;
}

Editor::Editor() noexcept : Application()
{
	std::cout << "Editor" << std::endl;

	Shader vShader;
	Shader fShader;

	vShader.CompileFromFile("Resources/Shaders/VShader.glsl", VERTEX_SHADER);
	fShader.CompileFromFile("Resources/Shaders/FShader.glsl", FRAGMENT_SHADER);

	m_pShaderProgramDefault = new ShaderProgram(vShader, fShader);

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

	this->m_pGridMesh = Mesh::CreateGrid(WIDTH, HEIGHT, DEPTH);

	m_pScene = new Scene();

	Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.01f, 100.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_pPerFrameUniform = new UniformBuffer(m_PerFrameArray, 1, sizeof(m_PerFrameArray));
	

	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	m_pPanel = new Panel(50, 50, 500, 500, new Texture2D("Resources/Textures/test3.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));

	m_pButton = new Button(25, 400, 200, 50, "Button 1", OnButtonPressed, OnButtonReleased);
	m_pButton->SetTexture(new Texture2D("Resources/Textures/test.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));
	m_pButton->SetOnPressedTexture(new Texture2D("Resources/Textures/test2.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));

	m_pButton2 = new Button(275, 400, 200, 50, "Button 2", OnButtonPressed, OnButtonReleased);
	m_pButton2->SetTexture(new Texture2D("Resources/Textures/test.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));
	m_pButton2->SetOnPressedTexture(new Texture2D("Resources/Textures/test2.png", TEX_FORMAT::TEX_FORMAT_RGBA, false));

	Texture2D* b = new Texture2D("Resources/Textures/test4.png", TEX_FORMAT::TEX_FORMAT_RGBA, false);
	Texture2D* f = new Texture2D("Resources/Textures/test5.png", TEX_FORMAT::TEX_FORMAT_RGBA, false);

	m_pSlider = new Slider(50, 50, 400, 30, b, f);
	m_pSlider->SetRatio(0.5F);
	m_pSlider->SetPercentage(1.0F);

	m_pSlider2 = new Slider(50, 100, 400, 30, b, f);
	m_pSlider2->SetRatio(0.5F);
	m_pSlider2->SetPercentage(0.0F);

	m_pPanel->Add(m_pButton);
	m_pPanel->Add(m_pButton2);
	m_pPanel->Add(m_pSlider);
	m_pPanel->Add(m_pSlider2);

	GetGUIManager().Add(m_pPanel);
	GetGUIManager().Add(m_pTextViewFPS);
	GetGUIManager().Add(m_pTextViewUPS);

	//GetContext().Enable(Cap::DEPTH_TEST);
}

Editor::~Editor()
{
	delete m_pShaderProgramDefault;
	delete m_pGridMesh;
	delete m_pScene;
	delete m_pTextViewFPS;
	delete m_pTextViewUPS;
}

void Editor::OnUpdate(float dtS)
{
	static float tempRotation = 0.0f;
	tempRotation += 1.0f * dtS;

	static float cameraSpeed = 5.0f;
	static float angularSpeed = 1.5f;

	if (Input::IsKeyDown(KEY_W))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Forward, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_S))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Backwards, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_A))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Left, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_D))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Right, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_E))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Up, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_Q))
	{
		m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Down, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_UP))
	{
		m_pScene->GetCamera().OffsetPitch(angularSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_DOWN))
	{
		m_pScene->GetCamera().OffsetPitch(-angularSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_LEFT))
	{
		m_pScene->GetCamera().OffsetYaw(-angularSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_RIGHT))
	{
		m_pScene->GetCamera().OffsetYaw(angularSpeed * dtS);
	}

	m_pScene->GetCamera().UpdateFromPitchYaw();
	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_pPerFrameUniform->UpdateData(&m_PerFrameArray);
	

	m_pTextViewFPS->SetText("FPS " + std::to_string(GetFPS()));
	m_pTextViewUPS->SetText("UPS " + std::to_string(GetUPS()));
}

void Editor::OnRender(float dtS)
{
	GetGraphicsContext().SetProgram(m_pShaderProgramDefault);
	GetGraphicsContext().SetUniformBuffer(m_pPerFrameUniform, 1);


	GetGraphicsContext().SetUniformBuffer(m_pGridUniform, 0);
	GetGraphicsContext().DrawMesh(*m_pGridMesh, PT_LINES);
}
