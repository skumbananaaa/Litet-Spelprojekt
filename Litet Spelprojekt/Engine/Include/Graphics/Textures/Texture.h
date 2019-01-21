#pragma once
#include <EnginePch.h>
#include <Defines.h>

enum TEX_PARAM : unsigned int
{
	TEX_PARAM_UNKNOWN = 0,
	TEX_PARAM_REPEAT = 1,
	TEX_NEAREST = 2,
	TEX_LINEAR = 3,
	TEX_MIPMAP_NEAREST = 4,
	TEX_MIPMAP_LINEAR = 5,
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
	unsigned int m_Texture;
};