#pragma once

#include <EnginePch.h>
#include <Graphics\Shaders\Shader.h>

class API ShaderProgram
{
	friend class GLContext;

public:
	ShaderProgram(const Shader& vShader, const Shader& fShader) noexcept;
	ShaderProgram(const Shader& vShader, const Shader& gShader, const Shader& fShader) noexcept;
	~ShaderProgram();

private:
	uint32 m_Program;
};