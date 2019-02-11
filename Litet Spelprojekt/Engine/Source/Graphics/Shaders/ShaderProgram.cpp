#include <EnginePch.h>
#include <Graphics\Shaders\ShaderProgram.h>

ShaderProgram::ShaderProgram(Shader* vShader, Shader* fShader) noexcept : 
	m_VShader(vShader),
	m_FShader(fShader)
{

}

ShaderProgram::ShaderProgram(const Shader& vShader) noexcept : 
	m_VShader(nullptr),
	m_FShader(nullptr)
{
	assert(vShader.m_Type == ShaderType::VERTEX_SHADER);

	GLint success;
	GLchar infoLog[512];

	m_Program = glCreateProgram();
	glAttachShader(m_Program, vShader.m_Shader);
	glLinkProgram(m_Program);

	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "ERROR LINKING SHADERS\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Created shaderprogram" << std::endl;
	}
}

ShaderProgram::ShaderProgram(const Shader& vShader, const Shader& fShader) noexcept : 
	m_VShader(nullptr),
	m_FShader(nullptr)
{
	assert(vShader.m_Type == ShaderType::VERTEX_SHADER);
	assert(fShader.m_Type == ShaderType::FRAGMENT_SHADER);

	GLint success;
	GLchar infoLog[512];

	m_Program = glCreateProgram();
	glAttachShader(m_Program, vShader.m_Shader);
	glAttachShader(m_Program, fShader.m_Shader);
	glLinkProgram(m_Program);

	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "ERROR LINKING SHADERS\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Created shaderprogram" << std::endl;
	}
}

ShaderProgram::ShaderProgram(const Shader& vShader, const Shader& gShader, const Shader& fShader) noexcept
{
	assert(vShader.m_Type == ShaderType::VERTEX_SHADER);
	assert(gShader.m_Type == ShaderType::GEOMETRY_SHADER);
	assert(fShader.m_Type == ShaderType::FRAGMENT_SHADER);

	GLint success;
	GLchar infoLog[512];

	m_Program = glCreateProgram();
	glAttachShader(m_Program, vShader.m_Shader);
	glAttachShader(m_Program, gShader.m_Shader);
	glAttachShader(m_Program, fShader.m_Shader);
	glLinkProgram(m_Program);

	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "ERROR LINKING SHADERS\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Created shaderprogram" << std::endl;
	}
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

	m_VShader->Construct();
	m_FShader->Construct();

	m_Program = glCreateProgram();
	glAttachShader(m_Program, m_VShader->m_Shader);
	glAttachShader(m_Program, m_FShader->m_Shader);
	glLinkProgram(m_Program);

	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "ERROR LINKING SHADERS\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Created shaderprogram" << std::endl;
	}

	Delete(m_VShader);
	Delete(m_FShader);
}

ShaderProgram* ShaderProgram::Create(Shader* vShader, Shader* fShader)
{
	assert(vShader->m_Type == ShaderType::VERTEX_SHADER);
	assert(fShader->m_Type == ShaderType::FRAGMENT_SHADER);
	return new ShaderProgram(vShader, fShader);
}