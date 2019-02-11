#include <EnginePch.h>
#include <Graphics\Shaders\Shader.h>

Shader::Shader() noexcept : m_Shader(0), m_type(ShaderType::UNDEFINED)
{

}

Shader::Shader(const std::string& shaderCode, ShaderType type) noexcept :
	m_Shader(0),
	m_type(type),
	m_ShaderCode(shaderCode)
{
	
}

Shader::~Shader()
{
	glDeleteShader(m_Shader);
}

bool Shader::CompileFromSource(const char* const source, ShaderType type) noexcept
{
	assert(type != ShaderType::UNDEFINED);
	m_type = type;

	GLint success;
	GLchar infoLog[512];

	m_Shader = glCreateShader(ShaderTypeTable(type));
	glShaderSource(m_Shader, 1, &source, NULL);
	glCompileShader(m_Shader);

	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
		std::cout << "ERROR COMPILING SHADER OF TYPE " << type << "\n" << infoLog << std::endl;
		return false;
	};

	return true;
}

bool Shader::CompileFromFile(const char* const path, ShaderType type) noexcept
{
	assert(type != ShaderType::UNDEFINED);
	m_type = type;

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
		return false;
	}

	const GLchar* shaderCode = shaderCodeString.c_str();
	
	m_Shader = glCreateShader(ShaderTypeTable(m_type));
	glShaderSource(m_Shader, 1, &shaderCode, NULL);
	glCompileShader(m_Shader);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
		std::cout << "ERROR COMPILING SHADER OF TYPE " << type << "\n" << infoLog << std::endl;
		return false;
	}

	return true;
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

	m_Shader = glCreateShader(ShaderTypeTable(m_type));
	glShaderSource(m_Shader, 1, &shaderCode, NULL);
	glCompileShader(m_Shader);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
		std::cout << "ERROR COMPILING SHADER OF TYPE " << m_type << "\n" << infoLog << std::endl;
	}
	m_ShaderCode = "";
}

Shader* Shader::Create(const char* const path, ShaderType type) noexcept
{
	assert(type != ShaderType::UNDEFINED);

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

	return new Shader(shaderCodeString, type);
}