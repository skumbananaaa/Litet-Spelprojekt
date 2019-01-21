#pragma once
#include <EnginePch.h>

enum TEX_PARAM : unsigned int
{
	TEX_PARAM_UNKNOWN = 0,
	TEX_PARAM_REPEAT = 1,
	TEX_NEAREST = 2,
	TEX_LINEAR = 3,
	TEX_NEAREST_MIPMAP_NEAREST = 4,
	TEX_LINEAR_MIPMAP_NEAREST = 5,
	TEX_NEAREST_MIPMAP_LINEAR = 6,
	TEX_LINEAR_MIPMAP_LINEAR = 7,
	TEX_PARAM_COUNT = 8,
};

enum TEX_FORMAT: unsigned int
{
	TEX_FORMAT_UNKNOWN = 0,
	TEX_FORMAT_R = 1,
	TEX_FORMAT_RG = 2,
	TEX_FORMAT_RGB = 3,
	TEX_FORMAT_RGBA = 4,
	TEX_FORMAT_COUNT = 5,
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

	Texture() noexcept;
	~Texture();

	void SetParameters(const TextureParams& params) noexcept;

protected:
	unsigned int m_Texture;

public:
	static unsigned int TexParamToGL(TEX_PARAM param) noexcept;
	static unsigned int TexFormatToGL(TEX_FORMAT param) noexcept;
	static unsigned int TexFormatToGLInternal(TEX_FORMAT param) noexcept;
};