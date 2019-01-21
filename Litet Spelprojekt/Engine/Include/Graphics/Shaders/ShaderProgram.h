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
	unsigned int m_Program;
};