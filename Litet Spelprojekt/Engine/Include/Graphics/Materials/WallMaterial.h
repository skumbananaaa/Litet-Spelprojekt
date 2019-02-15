#pragma once
#include "Material.h"

#define WALL_STUMP_FROM_CENTER 0.725f

_declspec(align(16)) struct DissolveBuffer
{
	glm::vec4 g_ClipDistances[3];
};

class API WallMaterial : public Material
{
	friend class ResourceHandler;

public:
	virtual void Bind(const Framebuffer* pGBuffer) const noexcept override;
	virtual void Unbind() const noexcept override;

	void SetClipPlane(const glm::vec4& plane, uint32 index) const noexcept;

private:
	WallMaterial();
	~WallMaterial();

	UniformBuffer* m_pDissolveBuffer;
	mutable DissolveBuffer m_Buffer;
};