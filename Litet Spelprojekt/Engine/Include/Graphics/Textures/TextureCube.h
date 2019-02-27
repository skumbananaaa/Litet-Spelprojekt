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

	virtual ~TextureCube();

private:
	TextureCube(const void* ppInitalData[6], const TextureDesc& desc, const TextureParams& params);
	TextureCube(const char* const paths[6], const TextureDesc& desc, const TextureParams& params);
	TextureCube(const Texture2D* tex);

	void CreateFromMemory(const void* ppInitalData[6], const TextureDesc& desc, const TextureParams& params);
	///<summary>Creates a texturecube out of 6 images given their filepaths</summary>
	void CreateFromFiles(const char* const ppPaths[6], const TextureDesc& desc, const TextureParams& params);
	///<summary>Creates a texturecube from a panorama (HDR) image</summary>
	void CreateFromPanorama(const Texture2D* tex);

public:
	///<summary>Creates a texturecube from an array. One for each face (6 faces)</summary>
	static TextureCube* CreateTextureCubeFromMemory(const void* ppInitalData[6], const TextureDesc& desc, const TextureParams& params = TextureParams());
	///<summary>Creates a texturecube from files. One for each face (6 faces)</summary>
	static TextureCube* CreateTextureCubeFromFiles(const char* const ppPaths[6], const TextureDesc& desc, const TextureParams& params = TextureParams());
	///<summary>Creates a texturecube from a panorama (HDR) image</summary>
	static TextureCube* CreateTextureCubeFromPanorama(const Texture2D* pPanorma);
};
