#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/Texture.h>

class API Texture2D : public Texture
{
public:
	Texture2D(Texture2D&& other) = delete;
	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(Texture2D&& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;

	Texture2D(const void* pInitalData, unsigned int format, unsigned int width, unsigned int height, const TextureParams& params = TextureParams());
	~Texture2D();

	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;
	unsigned int GetFormat() const noexcept;

private:
	void Create(const void* pInitalData, unsigned int format, unsigned int width, unsigned int height, const TextureParams& params);

private:
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Format;
};

inline unsigned int Texture2D::GetWidth() const noexcept
{
	return m_Width;
}

inline unsigned int Texture2D::GetHeight() const noexcept
{
	return m_Height;
}

inline unsigned int Texture2D::GetFormat() const noexcept
{
	return m_Format;
}