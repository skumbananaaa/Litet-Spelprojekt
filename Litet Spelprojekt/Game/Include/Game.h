#pragma once

#include <System\Application.h>
#include <glm.hpp>

class Game : Application
{
public:
	Game() noexcept;
	~Game();

	void OnUpdate(float dtS) override;
	void OnRender() override;

private:
	ShaderProgram* m_pShaderProgram;
	IndexedMesh* m_pTriangle;
};