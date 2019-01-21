#include "..\Include\Game.h"

Game::Game() noexcept
{
	Shader vShader;
	Shader fShader;

	vShader.CompileFromSource
	(
		"#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 normal;\n"
		"layout (location = 2) in vec3 tangent;\n"
		"layout (location = 3) in vec2 texCoords;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(position, 1.0);\n"
		"}\0",
		ShaderType::VERTEX_SHADER
	);

	fShader.CompileFromSource
	(
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0",
		ShaderType::FRAGMENT_SHADER
	);

	m_pShaderProgram = new ShaderProgram(vShader, fShader);

	Vertex triangleVertices[] = 
	{
		{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 1.0f) }
	};

	unsigned int triangleIndices[] =
	{
		0, 1, 2
	};

	m_pTriangle = new IndexedMesh(triangleVertices, triangleIndices, 3, 3);
}

Game::~Game()
{
	delete m_pShaderProgram;
	delete m_pTriangle;
}

void Game::OnUpdate(float dtS)
{
	Application::OnUpdate(dtS);
}

void Game::OnRender()
{
	Application::OnRender();

	GetContext().SetProgram(*m_pShaderProgram);
	GetContext().DrawIndexedMesh(*m_pTriangle);
}
