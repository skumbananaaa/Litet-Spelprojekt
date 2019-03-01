#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>

class API Room
{
public:
	Room(glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f)) noexcept;
	~Room();

	const StaticShadowCube* GetShadowMap() const noexcept;
	const glm::vec3& GetCenter() const noexcept;
	const bool IsActive() const noexcept;

	void SetActive(bool active) noexcept;
	void GenerateShadows(const Scene& scene) noexcept;

private:
	glm::vec3 m_Center;
	bool m_Active = false;
	StaticShadowCube* m_pShadowMap;
};