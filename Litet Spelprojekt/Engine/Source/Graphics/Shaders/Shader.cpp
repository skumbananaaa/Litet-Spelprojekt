#include <EnginePch.h>
#include <Graphics\Shaders\Shader.h>

Shader::Shader(const std::string& shaderCode, ShaderType type) noexcept 
	: m_Shader(0),
	m_Type(type),
	m_ShaderCode(shaderCode)
{
	
}

Shader::~Shader()
{
	glDeleteShader(m_Shader);
}

uint32 Shader::ShaderTypeTable(ShaderType type) const noexcept
{
	static uint32 shaderTypeTable[] =
	{
		0,
		GL_VERTEX_SHADER,
		GL_GEOMETRY_SHADER,
		GL_FRAGMENT_SHADER
	};

	return shaderTypeTable[type];
}

void Shader::Construct()
{
	const GLchar* shaderCode = m_ShaderCode.c_str();

	GLint success;
	GLchar infoLog[512];

	m_Shader = glCreateShader(ShaderTypeTable(m_Type));

	const char* pFinalSource = m_ShaderCode.c_str();
	glShaderSource(m_Shader, 1, &pFinalSource, NULL);
	glCompileShader(m_Shader);

	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
		std::cout << "ERROR COMPILING SHADER OF TYPE " << GetShaderTypeAsString(m_Type) << infoLog << std::endl;
	};
}

Shader* Shader::Create(const char* const path, ShaderType type, const std::string& defines) noexcept
{
	assert(type != ShaderType::UNDEFINED);

	std::string shaderTypeStr = GetShaderTypeAsString(type);
	std::string shaderCodeString;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(path);

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		shaderCodeString = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR READING SHADER FILE: " << path << std::endl;
		return nullptr;
	}

	return new Shader("#version 420\n#define " + shaderTypeStr + defines + shaderCodeString, type);
}