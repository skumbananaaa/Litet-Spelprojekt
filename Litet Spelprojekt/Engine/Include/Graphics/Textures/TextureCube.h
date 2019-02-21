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
	///<summary>
	///Basic constructor, provide 6 filepath, one for each face, use "CreateTextureFromPanorama" if possible.
	///</summary>

	/// <summary>
	/// <para>Basic constructor, provide 6 filepath, one for each face.</para>
	/// <para>Faces should be ordered as follows: Front, Back, Up, Down, Right, Left.</para>
	/// <para>If possible, use the function "CreateTextureFromPanorama" instead. </para>
	/// </summary>
	///<returns></returns>
	TextureCube(const char* const paths[6], TEX_FORMAT format, const TextureParams & params = TextureParams());
	/// <summary>
	/// <para>Basic constructor, provide a texture2D created from an HDR file.</para>
	/// <para>If possible, use the function "CreateTextureFromPanorama" instead. </para>
	/// </summary>
	///<returns></returns>
	TextureCube(const Texture2D * tex);
	virtual ~TextureCube();

	uint32 GetWidth() const noexcept;
	uint32 GetHeight() const noexcept;

private:
	//void Create(const void** ppInitalData, TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params);
	///<summary>Private constructor made so that a TextureCube can be created and then later modified into a correct state</summary>
	TextureCube();
	///<summary>Creates a texturecube out of 6 images given y their filepaths</summary>
	void Create(const char* const paths[6], TEX_FORMAT format, const TextureParams & params);
	///<summary>Creates a texturecube from a panorama (HDR) image</summary>
	void CreateFromPanorama(const Texture2D * tex);
	//Buffer to be sent into shader pass, only used in creating texturecube from HDR image.
	struct PanoramaBuff
	{
		glm::mat4 projection;
		glm::mat4 view;
	};

public:
	///<summary>Creates a texturecube from a panorama (HDR) image</summary>
	static TextureCube* CreateTextureFromPanorama(const Texture2D * tex);
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