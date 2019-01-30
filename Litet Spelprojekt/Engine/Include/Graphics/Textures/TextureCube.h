#pragma once
#include <Graphics/Textures/Texture.h>

class API TextureCube : public Texture
{
public:
	TextureCube(TextureCube&& other) = delete;
	TextureCube(const TextureCube& other) = delete;
	TextureCube& operator=(TextureCube&& other) = delete;
	TextureCube& operator=(const TextureCube& other) = delete;

	TextureCube(const void* ppInitalData[6], TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params = TextureParams());
	~TextureCube();

	uint32 GetWidth() const noexcept;
	uint32 GetHeight() const noexcept;

private:
	void Create(const void** ppInitalData, TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params);

private:
	uint32 m_Width;
	uint32 m_Height;
};


inline uint32 TextureCube::GetWidth() const noexcept
{
	return m_Width;
}

inline uint32 TextureCube::GetHeight() const noexcept
{
	return m_Height;
}