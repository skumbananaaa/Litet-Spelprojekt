#pragma once
#include <EnginePch.h>
#include <Defines.h>

enum TEX_PARAM : unsigned int
{
	TEX_PARAM_UNKNOWN = 0,
	TEX_PARAM_REPEAT = GL_REPEAT,
	TEX_NEAREST = GL_NEAREST,
	TEX_LINEAR = GL_LINEAR,
	TEX_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	TEX_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_NEAREST,
};

struct TextureParams
{
	TEX_PARAM Wrap = TEX_PARAM_REPEAT;
	TEX_PARAM MagFilter = TEX_NEAREST;
	TEX_PARAM MinFilter = TEX_NEAREST;
};

class API Texture
{
	friend class GLContext;

public:
	Texture(Texture&& other) = delete;
	Texture(const Texture& other) = delete;
	Texture& operator=(Texture&& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	Texture();
	~Texture();

	void SetParameters(const TextureParams& params) noexcept;

protected:
	GLuint m_Texture;
};