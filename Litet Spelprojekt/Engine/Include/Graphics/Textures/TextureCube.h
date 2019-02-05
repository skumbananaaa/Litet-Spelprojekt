#pragma once
#include <Graphics/Textures/Texture.h>
#include <Graphics/Textures/Texture2D.h>



class API TextureCube : public Texture
{
public:
	TextureCube(TextureCube&& other) = delete;
	TextureCube(const TextureCube& other) = delete;
	TextureCube& operator=(TextureCube&& other) = delete;
	TextureCube& operator=(const TextureCube& other) = delete;

	//TextureCube(const void* ppInitalData[6], TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params = TextureParams());
	TextureCube(const char* const paths[6], TEX_FORMAT format, const TextureParams & params = TextureParams());
	TextureCube(const Texture2D * tex);
	~TextureCube();

	uint32 GetWidth() const noexcept;
	uint32 GetHeight() const noexcept;

private:
	//void Create(const void** ppInitalData, TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params);

	///<summary>Creates a texturecube out of 6 images given y their filepaths</summary>
	void Create(const char* const paths[6], TEX_FORMAT format, const TextureParams & params);
	///<summary>Creates a texturecube from a panorama (HDR) image</summary>
	void CreateFromPanorama(const Texture2D * tex);

	struct PanoramaBuff
	{
		glm::mat4 projection;
		glm::mat4 view;
	};

public:
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