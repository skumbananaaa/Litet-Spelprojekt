#include <EnginePch.h>
#include <Graphics\Shaders\ShaderProgram.h>
#include <Graphics/Renderers/GLContext.h>

ShaderProgram::ShaderProgram(Shader* vShader, Shader* fShader) noexcept : 
	m_VShader(vShader),
	m_FShader(fShader)
{

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_Program);
}

int32 ShaderProgram::GetUniformLocation(const char* name) noexcept
{
	return glGetUniformLocation(m_Program, name);
}

void ShaderProgram::Construct()
{
	GLint success;
	GLchar infoLog[512];

	m_Program = glCreateProgram();

	m_VShader->Construct();
	m_VShader->SetDebugName((m_VShader->m_Path + " - VS").c_str());
	glAttachShader(m_Program, m_VShader->m_Shader);

	if (m_FShader)
	{
		m_FShader->Construct();
		m_FShader->SetDebugName((m_FShader->m_Path + " - FS").c_str());
		glAttachShader(m_Program, m_FShader->m_Shader);
	}

	glLinkProgram(m_Program);

	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "ERROR LINKING SHADERS:\n";
		if (m_VShader != nullptr)
		{
			std::cout << " VertexShader   : " << m_VShader->m_Path << std::endl;
		}
		if (m_FShader != nullptr)
		{
			std::cout << " FragmentShader : " << m_FShader->m_Path << std::endl;
		}
		std::cout << infoLog << std::endl;
	}
	else
	{
		std::cout << "Created shaderprogram" << std::endl;
	}

	DeleteSafe(m_VShader);
	DeleteSafe(m_FShader);
}

void ShaderProgram::SetDebugName(const char* pName)
{
	if (m_Program == 0)
	{
		return;
	}

	GL_CALL(glObjectLabel(GL_PROGRAM, m_Program, -1, pName));
}

ShaderProgram* ShaderProgram::Create(Shader* vShader, Shader* fShader)
{
	assert(vShader->m_Type == ShaderType::VERTEX_SHADER);
	if (fShader)
	{
		assert(fShader->m_Type == ShaderType::FRAGMENT_SHADER);
	}
	return new ShaderProgram(vShader, fShader);
}