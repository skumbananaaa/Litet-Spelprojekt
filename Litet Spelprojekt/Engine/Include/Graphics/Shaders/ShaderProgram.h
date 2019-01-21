#pragma once

#include <EnginePch.h>
#include <Defines.h>

class API ShaderProgram
{
	friend class GLContext;

public:
	ShaderProgram();
	~ShaderProgram();

private:
	GLuint m_Program;
};