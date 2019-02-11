#pragma once

#include <EnginePch.h>
#include <Graphics\Shaders\Shader.h>

class API ShaderProgram : IResource
{
	friend class GLContext;

public:
	ShaderProgram(const Shader& vShader) noexcept;
	ShaderProgram(const Shader& vShader, const Shader& fShader) noexcept;
	ShaderProgram(const Shader& vShader, const Shader& gShader, const Shader& fShader) noexcept;
	~ShaderProgram();

	int32 GetUniformLocation(const char* name) noexcept;

	virtual void Construct() override;

	static ShaderProgram* Create(Shader* vShader, Shader* fShader);

private:
	ShaderProgram(Shader* vShader, Shader* fShader) noexcept;

	uint32 m_Program;
	Shader* m_VShader;
	Shader* m_FShader;
};