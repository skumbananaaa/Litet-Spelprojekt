#pragma once
#include <Graphics/Textures/PlanarReflector.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include "Material.h"

_declspec(align(16)) struct WaterBuffer
{
	float DistortionFactor;
};

class API WaterMaterial : public Material
{
public:
	WaterMaterial();
	~WaterMaterial();

	void Bind() const noexcept override;
	void Unbind() const noexcept override;

	void SetDistortionTexture(Texture2D* pDistortion);
	void SetPlanarReflector(PlanarReflector* pReflector) const;
	void SetDistortionFactor(float distortionFactor) const;

private:
	Texture2D* m_pDistortion;
	Texture2D* m_pDepthMap;
	mutable UniformBuffer* m_pWaterBuffer;
	mutable PlanarReflector* m_pReflector;
	mutable WaterBuffer m_Buffer;
};