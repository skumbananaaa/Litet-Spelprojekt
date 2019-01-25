#include "..\Include\Editor.h"



Editor::Editor() noexcept : Application(true)
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

	m_pFontRenderer = FontRenderer::CreateFontRenderer(GetGraphicsContext(), "Resources/Fonts/arial.ttf", 800, 600);

	
	Camera* pCamera = new Camera(glm::vec3(-2.0F, 1.0F, 0.0F), -0.5f, 0.0f);
	pCamera->SetProjectionMatrix(glm::perspective(
		glm::radians<float>(90.0F),
		(float)GetWindow().GetWidth() /
		(float)GetWindow().GetHeight(),
		0.1F, 100.0F));
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_pPerFrameUniform = new UniformBuffer(m_PerFrameArray, 1, sizeof(m_PerFrameArray));
	
	//GetContext().Enable(Cap::DEPTH_TEST);
}

Editor::~Editor()
{
	delete m_pShaderProgramDefault;
	delete m_pGridMesh;
	delete m_pScene;
	delete m_pFontRenderer;
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
	
	Application::OnUpdate(dtS);
}

void Editor::OnRender()
{
	GetGraphicsContext().SetProgram(m_pShaderProgramDefault);
	GetGraphicsContext().SetUniformBuffer(m_pPerFrameUniform, 1);


	GetGraphicsContext().SetUniformBuffer(m_pGridUniform, 0);
	GetGraphicsContext().DrawMesh(*m_pGridMesh, PT_LINES);


	m_pFontRenderer->RenderText(GetGraphicsContext(), "FPS " + std::to_string(GetFPS()), 0.0f, 570.0f, 0.5f);
	m_pFontRenderer->RenderText(GetGraphicsContext(), "UPS " + std::to_string(GetUPS()), 0.0f, 540.0f, 0.5f);

	Application::OnRender();
}
