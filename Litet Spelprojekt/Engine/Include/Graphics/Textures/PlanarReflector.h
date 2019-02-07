#pragma once
#include "Framebuffer.h"

#define REFLECTION_SIZE 384

class API PlanarReflector
{
	friend class DefferedRenderer;

public:
	PlanarReflector(PlanarReflector&& other) = delete;
	PlanarReflector(const PlanarReflector& other) = delete;
	PlanarReflector& operator=(PlanarReflector&& other) = delete;
	PlanarReflector& operator=(const PlanarReflector& other) = delete;

	PlanarReflector(const glm::vec3& normal, float distFromOrigin) noexcept;
	~PlanarReflector();

	void SetPlane(const glm::vec3& normal, float distFromOrigin) noexcept;

private:
	void Create() noexcept;
	Framebuffer* GetFramebuffer() const noexcept;
	Texture2D* GetReflectionTexture() const noexcept;

private:
	Framebuffer* m_pFramebuffer;
	glm::vec3 m_Normal;
	float m_Distance;
};