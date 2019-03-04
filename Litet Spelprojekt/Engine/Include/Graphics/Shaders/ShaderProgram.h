#pragma once
#include <EnginePch.h>
#include <Graphics\Shaders\Shader.h>

class API ShaderProgram : public IResource, public IGLObject
{
	friend class GLContext;
	friend class ResourceHandler;

public:
	int32 GetUniformLocation(const char* name) noexcept;

	virtual void Construct() override;
	virtual void SetDebugName(const char* pName) override;

private:
	ShaderProgram(Shader* vShader, Shader* fShader) noexcept;
	~ShaderProgram();

	static ShaderProgram* Create(Shader* vShader, Shader* fShader);

	uint32 m_Program;
	Shader* m_VShader;
	Shader* m_FShader;
};