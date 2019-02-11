#include <EnginePch.h>
#include <Graphics\Shaders\Shader.h>

Shader::Shader() noexcept : m_Shader(0), m_type(ShaderType::UNDEFINED)
{
}

Shader::~Shader()
{
	glDeleteShader(m_Shader);
}

bool Shader::CompileFromSource(const char* const pSource, ShaderType type, const ShaderDefines& defines) noexcept
{
	assert(type != ShaderType::UNDEFINED);
	m_type = type;

	std::string shaderTypeStr;
	if (type == VERTEX_SHADER)
	{
		shaderTypeStr = "VERTEX_SHADER\n";
	}
	else if (type == GEOMETRY_SHADER)
	{
		shaderTypeStr = "GEOMETRY_SHADER\n";
	}
	else if (type == FRAGMENT_SHADER)
	{
		shaderTypeStr = "FRAGMENT_SHADER\n";
	}

	std::string finalSource = "#define " + shaderTypeStr;
	for (uint32 i = 0; i < defines.NumDefines; i++)
	{
		finalSource += "#define " + std::string(defines.ppDefines[i]) + '\n';
	}
	finalSource += pSource;

	GLint success;
	GLchar infoLog[512];

	m_Shader = glCreateShader(ShaderTypeTable(type));

	const char* pFinalSource = finalSource.c_str();
	glShaderSource(m_Shader, 1, &pFinalSource, NULL);
	glCompileShader(m_Shader);

	glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_Shader, 512, NULL, infoLog);
		std::cout << "ERROR COMPILING SHADER OF TYPE " << shaderTypeStr << infoLog << std::endl;
		return false;
	};

	return true;
}

bool Shader::CompileFromFile(const char* const path, ShaderType type, const ShaderDefines& defines) noexcept
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

	return CompileFromSource(shaderCodeString.c_str(), type, defines);
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
