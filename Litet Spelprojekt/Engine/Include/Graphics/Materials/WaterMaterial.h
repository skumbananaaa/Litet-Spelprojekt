#pragma once
#include <Graphics/Textures/PlanarReflector.h>
#include "CustomMaterial.h"

class WaterMaterial : public CustomMaterial
{
public:
	WaterMaterial();
	~WaterMaterial();

	void Bind() const noexcept override;
	void Unbind() const noexcept override;

	void SetPlanarReflector(PlanarReflector* pReflector);
	void SetDistortionTexture(Texture2D* pDistortion);
	void SetNormalMap(Texture2D* pNormalMap);

private:
	PlanarReflector* m_pReflector;
	Texture2D* m_pDistortion;
	Texture2D* m_pNormalMap;
	Texture2D* m_pDepthMap;
};