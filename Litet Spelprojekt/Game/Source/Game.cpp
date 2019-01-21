#include "..\Include\Game.h"

Game::Game() noexcept
{
	Shader vShader;
	Shader fShader;

	vShader.CompileFromSource
	(
		"#version 420 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 normal;\n"
		"layout (location = 2) in vec3 tangent;\n"
		"layout (location = 3) in vec2 texCoords;\n"
		"layout (std140, binding = 0) uniform PerObjectBlock\n"
		"{\n"
		"	mat4 transform;\n"
		"};\n"
		"layout (std140, binding = 1) uniform PerFrameBlock\n"
		"{\n"
		"	mat4 cameraCombined;\n"
		"};\n"
		"void main()\n"
		"{\n"
		"   gl_Position = cameraCombined * transform * vec4(position, 1.0);\n"
		"}\0",
		ShaderType::VERTEX_SHADER
	);

	fShader.CompileFromSource
	(
		"#version 420 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0",
		ShaderType::FRAGMENT_SHADER
	);

	m_pShaderProgram = new ShaderProgram(vShader, fShader);

	Vertex triangleVertices[] = 
	{
		// Front (Seen from front)
		{ glm::vec3(-1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Top (Seen from above)
		{ glm::vec3(-1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Back (Seen from front)
		{ glm::vec3(-1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Bottom (Seen from above)
		{ glm::vec3(-1.0F, -1.0F, -1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F, -1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F,  1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Left (Seen from left)
		{ glm::vec3(-1.0F,  1.0F, -1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(-1.0F,  1.0F,  1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(-1.0F, -1.0F,  1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F, -1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 0.0F) },

		// Right (Seen from left)
		{ glm::vec3(1.0F,  1.0F, -1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(1.0F, -1.0F, -1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 0.0F) }
	};

	unsigned int triangleIndices[] =
	{
		// Front (Seen from front)
		0, 2, 1,
		2, 0, 3,

		// Top (Seen from above)
		4, 6, 5,
		6, 4, 7,

		// Back (Seen from front)
		8, 9, 10,
		10, 11, 8,

		// Bottom (Seen from above)
		12, 13, 14,
		14, 15, 12,

		// Left (Seen from left)
		16, 18, 17,
		18, 16, 19,

		// Right (Seen from left)
		20, 21, 22,
		22, 23, 20
	};

	m_pTestMesh = new IndexedMesh(triangleVertices, triangleIndices, 24, 36);

	m_pScene = new Scene();

	GameObject* pGameObject = nullptr;
	for (unsigned int i = 0; i < 9; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(m_pTestMesh);
		pGameObject->SetPosition(glm::vec3(i % 3, 0.0f, i / 3));
		pGameObject->SetScale(glm::vec3(0.2f));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
		m_GameObjectUniforms.push_back(new UniformBuffer(glm::value_ptr(pGameObject->GetTransform()), 1, sizeof(glm::mat4)));
	}

	Camera* pCamera = new Camera(glm::vec3(-2.0F, 1.0F, 0.0F), -0.5f, 0.0f);
	pCamera->SetProjectionMatrix(glm::perspective(
		glm::radians<float>(90.0F),
		(float)GetWindow().GetWidth() /
		(float)GetWindow().GetHeight(),
		0.1F, 100.0F));
	pCamera->Update();
	m_pScene->SetCamera(pCamera);

	m_pCameraUniform = new UniformBuffer(glm::value_ptr(pCamera->GetCombinedMatrix()), 1, sizeof(glm::mat4));
}

Game::~Game()
{
	delete m_pShaderProgram;
	delete m_pTestMesh;
	delete m_pScene;
}

void Game::OnUpdate(float dtS)
{
	static float tempRotation = 0.0f;
	tempRotation += 1.0f * dtS;

	for (unsigned int i = 0; i < 9; i++)
	{
		m_pScene->GetGameObjects()[i]->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, tempRotation));
		m_pScene->GetGameObjects()[i]->UpdateTransform();
		m_GameObjectUniforms[i]->UpdateData(glm::value_ptr(m_pScene->GetGameObjects()[i]->GetTransform()));
	}
	
	m_pScene->GetCamera().Update();
	m_pCameraUniform->UpdateData(glm::value_ptr(m_pScene->GetCamera().GetCombinedMatrix()));

	Application::OnUpdate(dtS);
}

void Game::OnRender()
{
	GetContext().SetProgram(*m_pShaderProgram);
	GetContext().SetUniformBuffer(*m_pCameraUniform, 1);

	for (unsigned int i = 0; i < 9; i++)
	{
		GetContext().SetUniformBuffer(*m_GameObjectUniforms[i], 0);
		GetContext().DrawIndexedMesh(m_pScene->GetGameObjects()[i]->GetMesh());
	}

	Application::OnRender();
}
