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

class API Shader : public IResource
{
	friend class ShaderProgram;

public:
	Shader() noexcept;
	~Shader();

	bool CompileFromSource(const char* const source, ShaderType type) noexcept;
	bool CompileFromFile(const char* const path, ShaderType type) noexcept;

	uint32 ShaderTypeTable(ShaderType type) const noexcept;
	virtual void Construct() override;

	static Shader* Create(const char* const path, ShaderType type) noexcept;

private:
	Shader(const std::string& shaderCode, ShaderType type) noexcept;

	uint32 m_Shader;
	ShaderType m_type;
	std::string m_ShaderCode;
};