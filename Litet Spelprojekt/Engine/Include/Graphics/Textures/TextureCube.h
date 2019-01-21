#pragma once
#include "Texture.h"

class API TextureCube : public Texture
{
public:
	TextureCube(TextureCube&& other) = delete;
	TextureCube(const TextureCube& other) = delete;
	TextureCube& operator=(TextureCube&& other) = delete;
	TextureCube& operator=(const TextureCube& other) = delete;

	TextureCube(const void* ppInitalData[6], TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params = TextureParams());
	~TextureCube();

	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	unsigned int GetFormat() const noexcept;

private:
	void Create(const void** ppInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params);

private:
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Format;
};


inline unsigned int TextureCube::GetWidth() const noexcept
{
	return m_Width;
}

inline unsigned int TextureCube::GetHeight() const noexcept
{
	return m_Height;
}

inline unsigned int TextureCube::GetFormat() const noexcept
{
	return m_Format;
}
