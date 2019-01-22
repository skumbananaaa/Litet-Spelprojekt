#pragma once
#include <EnginePch.h>

enum TEX_PARAM : uint32
{
	TEX_PARAM_UNKNOWN = 0,
	TEX_PARAM_REPEAT = 1,
	TEX_PARAM_EDGECLAMP = 2, //Sets to textureedge
	TEX_PARAM_BORDERCLAMP = 3, //Sets to a color
	TEX_NEAREST = 4,
	TEX_LINEAR = 5,
	TEX_NEAREST_MIPMAP_NEAREST = 6,
	TEX_LINEAR_MIPMAP_NEAREST = 7,
	TEX_NEAREST_MIPMAP_LINEAR = 8,
	TEX_LINEAR_MIPMAP_LINEAR = 9,
	TEX_PARAM_COUNT = 10,
};

enum TEX_FORMAT: uint32
{
	TEX_FORMAT_UNKNOWN = 0,
	TEX_FORMAT_R = 1,
	TEX_FORMAT_RG = 2,
	TEX_FORMAT_RGB = 3,
	TEX_FORMAT_RGBA = 4,
	TEX_FORMAT_RGBA16F = 5,
	TEX_FORMAT_DEPTH = 6,
	TEX_FORMAT_DEPTH_STENCIL = 7,
	TEX_FORMAT_COUNT = 8,
};

struct TextureParams
{
	TEX_PARAM Wrap = TEX_PARAM_EDGECLAMP;
	TEX_PARAM MagFilter = TEX_NEAREST;
	TEX_PARAM MinFilter = TEX_NEAREST;
};

class API Texture
{
	friend class GLContext;
	friend class Framebuffer;

public:
	Texture(Texture&& other) = delete;
	Texture(const Texture& other) = delete;
	Texture& operator=(Texture&& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	Texture() noexcept;
	~Texture();

	void SetParameters(const TextureParams& params) noexcept;
	TEX_FORMAT GetFormat() const noexcept;

protected:
	TEX_FORMAT m_Format;
	uint32 m_Texture;

public:
	static uint32 TexParamToGL(TEX_PARAM param) noexcept;
	static uint32 TexFormatToGL(TEX_FORMAT param) noexcept;
	static uint32 TexFormatToGLInternal(TEX_FORMAT param) noexcept;
	static uint32 TexFormatToGLType(TEX_FORMAT param) noexcept;
};

inline TEX_FORMAT Texture::GetFormat() const noexcept
{
	return m_Format;
}