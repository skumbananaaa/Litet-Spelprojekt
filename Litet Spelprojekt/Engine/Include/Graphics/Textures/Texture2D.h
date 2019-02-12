#pragma once
#include <Graphics/Textures/Texture.h>
#include <IO/IResource.h>

class API Texture2D : public Texture, public IResource
{
	friend class TEXTURE;
	friend class ResourceHandler;

public:
	Texture2D(Texture2D&& other) = delete;
	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(Texture2D&& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;

	Texture2D(const void* pInitalData, const TextureDesc& desc, const TextureParams& params = TextureParams());
	~Texture2D();

	uint32 GetWidth() const noexcept;
	uint32 GetHeight() const noexcept;
	uint32 GetSamples() const noexcept;

private:
	Texture2D(const char* const path, TEX_FORMAT format, bool generateMipmaps = true, bool flipVertically = false, const TextureParams& params = TextureParams());

private:
	static Texture2D* CreateTextureFromFile(const char* pPath, TEX_FORMAT format, bool generateMipmaps, bool flipVertically, const TextureParams& params);

	void Create(const void* pInitalData, const TextureDesc& desc, const TextureParams& params);
	void Create(const char* pPath, TEX_FORMAT format, bool generateMipmaps, bool flipVertically, const TextureParams& params);
	void CreateMS(const TextureDesc& desc, const TextureParams& params);
	virtual void Construct() override;

private:
	int32 m_Width;
	int32 m_Height;
	uint32 m_Samples;
	TextureParams m_Params;
	bool m_GenerateMipmaps;
	void* m_pTextureData;

public:
	static Texture2D* CreateTextureFromMemory(const void* pInitalData, const TextureDesc& desc, const TextureParams& params);
};

inline uint32 Texture2D::GetWidth() const noexcept
{
	return m_Width;
}

inline uint32 Texture2D::GetHeight() const noexcept
{
	return m_Height;
}

inline uint32 Texture2D::GetSamples() const noexcept
{
	return m_Samples;
}