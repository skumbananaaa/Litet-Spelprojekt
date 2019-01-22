#include <EnginePch.h>
#include <Graphics\Shaders\ShaderProgram.h>

ShaderProgram::ShaderProgram(const Shader& vShader, const Shader& fShader) noexcept
{
	assert(vShader.m_type == ShaderType::VERTEX_SHADER);
	assert(fShader.m_type == ShaderType::FRAGMENT_SHADER);

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
	assert(vShader.m_type == ShaderType::VERTEX_SHADER);
	assert(gShader.m_type == ShaderType::GEOMETRY_SHADER);
	assert(fShader.m_type == ShaderType::FRAGMENT_SHADER);

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
