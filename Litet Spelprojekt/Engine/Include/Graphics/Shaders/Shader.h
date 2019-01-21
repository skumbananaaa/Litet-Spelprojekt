#pragma once

#include <EnginePch.h>
#include <Defines.h>

enum ShaderType : unsigned char
{
	UNDEFINED = 0,
	VERTEX_SHADER = 1,
	GEOMETRY_SHADER = 2,
	FRAGMENT_SHADER = 3
};

class API Shader
{
	friend class ShaderProgram;

public:
	Shader() noexcept;
	~Shader();

	bool CompileFromSource() noexcept;
	bool CompileFromFile(const char* const path, ShaderType type) noexcept;

	unsigned int ShaderTypeTable(ShaderType type) const noexcept;

private:
	unsigned int m_Shader;
	ShaderType m_type;
};