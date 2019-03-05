#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>

class API Room
{
public:
	Room(const Room& other) = delete;
	Room& operator=(Room&& other) = delete;
	Room& operator=(const Room& other) = delete;

	Room(Room&& other);
	Room() noexcept;
	Room(glm::vec3 center) noexcept;
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