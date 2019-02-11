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

class API Shader : public IResource
{
	friend class ShaderProgram;

public:
	Shader() noexcept;
	~Shader();

	bool CompileFromSource(const char* const source, ShaderType type, const ShaderDefines& defines = ShaderDefines()) noexcept;
	bool CompileFromFile(const char* const path, ShaderType type, const ShaderDefines& defines = ShaderDefines()) noexcept;

	uint32 ShaderTypeTable(ShaderType type) const noexcept;
	virtual void Construct() override;

	static Shader* Create(const char* const path, ShaderType type, const ShaderDefines& defines = ShaderDefines()) noexcept;

private:
	Shader(const std::string& shaderCode, ShaderType type, const ShaderDefines& defines) noexcept;

	uint32 m_Shader;
	ShaderType m_Type;
	std::string m_ShaderCode;
	ShaderDefines m_Defines;
};