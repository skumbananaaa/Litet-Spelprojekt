#pragma once
#include <Graphics/Textures/Texture.h>

class API Texture2D : public Texture
{
public:
	Texture2D(Texture2D&& other) = delete;
	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(Texture2D&& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;

	Texture2D(const void* pInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params = TextureParams());
	~Texture2D();

	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	TEX_FORMAT GetFormat() const noexcept;

private:
	void Create(const void* pInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params);

private:
	unsigned int m_Width;
	unsigned int m_Height;
	TEX_FORMAT m_Format;
};

inline unsigned int Texture2D::GetWidth() const noexcept
{
	return m_Width;
}

inline unsigned int Texture2D::GetHeight() const noexcept
{
	return m_Height;
}

inline TEX_FORMAT Texture2D::GetFormat() const noexcept
{
	return m_Format;
}