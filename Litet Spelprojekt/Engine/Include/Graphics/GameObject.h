#pragma once

#include <EnginePch.h>
#include <GLM\gtc\matrix_transform.hpp>
#include <Graphics\Geometry\IndexedMesh.h>

class API GameObject
{
public:
	GameObject() noexcept;
	~GameObject();

	void SetMesh(const IndexedMesh* const mesh) noexcept;
	void SetPosition(const glm::vec3& position) noexcept;
	void SetRotation(const glm::vec4& rotation) noexcept;
	void SetScale(const glm::vec3& scale) noexcept;

	const IndexedMesh& GetMesh() const noexcept;
	const glm::mat4& GetTransform() const noexcept;

	void UpdateTransform() noexcept;

private:
	const IndexedMesh* m_pMesh;
	glm::vec3 m_position;
	glm::vec4 m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_transform;
	bool m_IsDirty;
};

inline const IndexedMesh& GameObject::GetMesh() const noexcept
{
	return *m_pMesh;
}

inline const glm::mat4& GameObject::GetTransform() const noexcept
{
	return m_transform;
}