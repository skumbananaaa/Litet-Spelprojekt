#pragma once
#include <Graphics/Buffers/UniformBuffer.h>
#include "Material.h"

_declspec(align(16)) struct WaterIndoorBuffer
{
	float DistortionFactor;
};

class API WaterIndoorMaterial : public Material
{
	friend class ResourceHandler;

public:
	void Bind(const Framebuffer* pGBuffer) const noexcept override;
	void Unbind() const noexcept override;

	void SetDistortionTexture(Texture2D* pDistortion);
	void SetDistortionFactor(float distortionFactor) const;

private:
	WaterIndoorMaterial();
	~WaterIndoorMaterial();

	Texture2D* m_pDistortion;
	Texture2D* m_pDepthMap;
	mutable UniformBuffer* m_pWaterBuffer;
	mutable WaterIndoorBuffer m_Buffer;
};