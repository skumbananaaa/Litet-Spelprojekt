#pragma once
#include <EnginePch.h>

enum TEX_PARAM : uint32
{
	TEX_PARAM_UNKNOWN = 0,
	TEX_PARAM_REPEAT = 1,
	TEX_PARAM_EDGECLAMP = 2, //Sets to textureedge
	TEX_PARAM_BORDERCLAMP = 3, //Sets to a color
	TEX_PARAM_NEAREST = 4,
	TEX_PARAM_LINEAR = 5,
	TEX_PARAM_NEAREST_MIPMAP_NEAREST = 6,
	TEX_PARAM_LINEAR_MIPMAP_NEAREST = 7,
	TEX_PARAM_NEAREST_MIPMAP_LINEAR = 8,
	TEX_PARAM_LINEAR_MIPMAP_LINEAR = 9,
	TEX_PARAM_COUNT = 10,
};

enum TEX_FORMAT : uint32
{
	TEX_FORMAT_UNKNOWN = 0,
	TEX_FORMAT_R = 1,
	TEX_FORMAT_R16F = 2,
	TEX_FORMAT_R32F = 3,
	TEX_FORMAT_RG = 4,
	TEX_FORMAT_RGB = 5,
	TEX_FORMAT_RGBA = 6,
	TEX_FORMAT_RGBA16F = 7,
	TEX_FORMAT_DEPTH = 8,
	TEX_FORMAT_DEPTH_STENCIL = 9,
	TEX_FORMAT_RGB16F = 10,
	TEX_FORMAT_COUNT = 11,
};

struct TextureParams
{
	TEX_PARAM Wrap = TEX_PARAM_EDGECLAMP;
	TEX_PARAM MagFilter = TEX_PARAM_NEAREST;
	TEX_PARAM MinFilter = TEX_PARAM_NEAREST;
};

struct TextureDesc
{
	TEX_FORMAT Format = TEX_FORMAT_UNKNOWN;
	uint32 Width = 0;
	uint32 Height = 0;
	uint32 Samples = 1;
	bool GenerateMips = true;
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
	virtual ~Texture();

	void SetParameters(const TextureParams& params) noexcept;
	TEX_FORMAT GetFormat() const noexcept;

private:
	uint32 GetType() const noexcept;

protected:
	TEX_FORMAT m_Format;
	uint32 m_Texture;
	uint32 m_Type;

public:
	static uint32 TexParamToGL(TEX_PARAM param) noexcept;
	static uint32 TexFormatToGL(TEX_FORMAT format) noexcept;
	static uint32 TexFormatToGLInternal(TEX_FORMAT format) noexcept;
	static uint32 TexFormatToGLType(TEX_FORMAT format) noexcept;
	static uint32 FormatToNrChannels(TEX_FORMAT format) noexcept;
};

inline TEX_FORMAT Texture::GetFormat() const noexcept
{
	return m_Format;
}

inline uint32 Texture::GetType() const noexcept
{
	return m_Type;
}