#pragma once
#include <Graphics/Textures/PlanarReflector.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include "Material.h"

_declspec(align(16)) struct WaterOutdoorBuffer
{
	glm::vec2 WaveFactor;
	glm::vec2 IcebergPos;
};

class API WaterOutdoorMaterial : public Material
{
	friend class ResourceHandler;

public:
	void Bind(const Framebuffer* pGBuffer) const noexcept override;
	void Unbind() const noexcept override;

	void SetPlanarReflector(PlanarReflector* pReflector) const;
	void SetWaveFactor(const glm::vec2& waveFactor) const;
	void SetIcebergPosition(const glm::vec2& pos) const;

private:
	WaterOutdoorMaterial();
	~WaterOutdoorMaterial();

	mutable bool m_LastDepthMask;
	mutable Func m_LastDepthFunc;
	mutable UniformBuffer* m_pWaterBuffer;
	mutable PlanarReflector* m_pReflector;
	mutable WaterOutdoorBuffer m_Buffer;
};