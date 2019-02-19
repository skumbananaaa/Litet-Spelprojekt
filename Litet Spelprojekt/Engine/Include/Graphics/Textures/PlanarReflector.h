#pragma once
#include "Framebuffer.h"

#define REFLECTION_SIZE 512

class API PlanarReflector
{
public:
	PlanarReflector(PlanarReflector&& other) = delete;
	PlanarReflector(const PlanarReflector& other) = delete;
	PlanarReflector& operator=(PlanarReflector&& other) = delete;
	PlanarReflector& operator=(const PlanarReflector& other) = delete;

	PlanarReflector(const glm::vec3& normal, float distFromOrigin) noexcept;
	~PlanarReflector();

	void SetPlane(const glm::vec3& normal, float distFromOrigin) noexcept;
	
	Framebuffer* GetFramebuffer() const noexcept;
	Texture2D* GetReflectionTexture() const noexcept;
	const glm::vec4& GetLevelClipPlane() const noexcept;

private:
	void Create() noexcept;

private:
	Framebuffer* m_pFramebuffer;
	glm::vec4 m_ClipPlane;
};