#pragma once

#include <EnginePch.h>
#include <Defines.h>

class API ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void Bind();
	void Unbind();

private:
	GLuint m_Program;
};