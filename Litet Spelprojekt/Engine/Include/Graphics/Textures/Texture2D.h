#pragma once
#include <Graphics/Textures/Texture.h>

class API Texture2D : public Texture
{
public:
	Texture2D(Texture2D&& other) = delete;
	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(Texture2D&& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;

	Texture2D(const void* pInitalData, TEX_FORMAT format, uint32 width, uint32 height, bool generateMipmaps = true, const TextureParams& params = TextureParams());
	~Texture2D();

	uint32 GetWidth() const noexcept;
	uint32 GetHeight() const noexcept;

private:
	void Create(const void* pInitalData, TEX_FORMAT format, uint32 width, uint32 height, bool generateMipmaps, const TextureParams& params);

private:
	uint32 m_Width;
	uint32 m_Height;
};

inline uint32 Texture2D::GetWidth() const noexcept
{
	return m_Width;
}

inline uint32 Texture2D::GetHeight() const noexcept
{
	return m_Height;
}