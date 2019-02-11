#pragma once
#include "Material.h"

#define WALL_STUMP_FROM_CENTER 0.725f

_declspec(align(16)) struct DissolveBuffer
{
	glm::vec4 g_ClipDistances[3];
	float DissolveFactor;
};

class API WallMaterial : public Material
{
public:
	WallMaterial();
	~WallMaterial();

	virtual void Bind(const Framebuffer* pGBuffer) const noexcept override;
	virtual void Unbind() const noexcept override;

	void SetDissolveFactor(float dissolveFactor) const noexcept;
	void SetClipPlane(const glm::vec4& plane, uint32 index) const noexcept;

private:
	UniformBuffer* m_pDissolveBuffer;
	mutable DissolveBuffer m_Buffer;
};