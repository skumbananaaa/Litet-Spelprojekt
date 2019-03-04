#pragma once
#include "TextureCube.h"
#include "Framebuffer.h"

class PointLight;
class Scene;

class API StaticShadowCube
{
public:
	StaticShadowCube(StaticShadowCube&& other) = delete;
	StaticShadowCube(const StaticShadowCube& other) = delete;
	StaticShadowCube& operator=(StaticShadowCube&& other) = delete;
	StaticShadowCube& operator=(const StaticShadowCube& other) = delete;

	StaticShadowCube(const glm::vec3& position, const Scene& scene);
	~StaticShadowCube();

	const TextureCube* GetCubeTexture() const;
	constexpr float GetFarPlane() const;

private:
	void Create(const glm::vec3& position, const Scene& scene);

private:
	TextureCube* m_pCube;
};

inline const TextureCube* StaticShadowCube::GetCubeTexture() const
{
	return m_pCube;
}

inline constexpr float StaticShadowCube::GetFarPlane() const
{
	return 50.0f;
}
