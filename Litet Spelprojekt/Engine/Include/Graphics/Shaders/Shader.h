#pragma once
#include <EnginePch.h>
#include <IO/IResource.h>

enum ShaderType : uint8
{
	UNDEFINED = 0,
	VERTEX_SHADER = 1,
	GEOMETRY_SHADER = 2,
	FRAGMENT_SHADER = 3
};

struct ShaderDefines
{
	const char** ppDefines = nullptr;
	uint32 NumDefines = 0;
};

inline std::string ParseDefines(const ShaderDefines& defines)
{
	std::string result;
	for (uint32 i = 0; i < defines.NumDefines; i++)
	{
		result += "#define " + std::string(defines.ppDefines[i]) + '\n';
	}

	return result;
}

inline std::string GetShaderTypeAsString(ShaderType type)
{
	if (type == VERTEX_SHADER)
	{
		return "VERTEX_SHADER\n";
	}
	else if (type == GEOMETRY_SHADER)
	{
		return "GEOMETRY_SHADER\n";
	}
	else if (type == FRAGMENT_SHADER)
	{
		return "FRAGMENT_SHADER\n";
	}

	return "UNKNOWN\n";
}

class API Shader : public IResource
{
	friend class ShaderProgram;

public:
	uint32 ShaderTypeTable(ShaderType type) const noexcept;
	virtual void Construct() override;

	static Shader* Create(const char* const path, ShaderType type, const std::string& = "") noexcept;

private:
	Shader(const std::string& shaderCode, ShaderType type) noexcept;
	Shader() noexcept;
	~Shader();

	uint32 m_Shader;
	ShaderType m_Type;
	std::string m_ShaderCode;
};