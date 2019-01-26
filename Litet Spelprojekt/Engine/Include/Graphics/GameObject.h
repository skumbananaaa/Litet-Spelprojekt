#pragma once

#include <EnginePch.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/Decal.h>

class API GameObject
{
public:
	GameObject() noexcept;
	~GameObject();

	void SetMesh(const IndexedMesh* const pMesh) noexcept;
	void SetMaterial(const Material* const pMaterial) noexcept;
	void SetDecal(const Decal* const pDecal) noexcept;
	void SetPosition(const glm::vec3& position) noexcept;
	void SetRotation(const glm::vec4& rotation) noexcept;
	void SetScale(const glm::vec3& scale) noexcept;

	const Decal& GetDecal() const noexcept;
	const Material& GetMaterial() const noexcept;
	const IndexedMesh& GetMesh() const noexcept;
	const glm::mat4& GetTransform() const noexcept;

	bool HasMaterial() const noexcept;
	bool HasDecal() const noexcept;
	bool HasMesh() const noexcept;

	const glm::vec3& GetPosition() const noexcept;

	void UpdateTransform() noexcept;

private:
	const IndexedMesh* m_pMesh;
	const Material* m_pMaterial;
	const Decal* m_pDecal;
	glm::vec3 m_position;
	glm::vec4 m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_transform;
	bool m_IsDirty;
};

inline const glm::vec3& GameObject::GetPosition() const noexcept
{
	return m_position;
}

inline void GameObject::SetDecal(const Decal* const pDecal) noexcept
{
	m_pDecal = pDecal;
}

inline void GameObject::SetMaterial(const Material* const pMaterial) noexcept
{
	m_pMaterial = pMaterial;
}

inline const Decal& GameObject::GetDecal() const noexcept
{
	assert(m_pDecal != nullptr);
	return *m_pDecal;
}

inline const Material& GameObject::GetMaterial() const noexcept
{
	assert(m_pMaterial != nullptr);
	return *m_pMaterial;
}

inline const IndexedMesh& GameObject::GetMesh() const noexcept
{
	assert(m_pMesh != nullptr);
	return *m_pMesh;
}

inline const glm::mat4& GameObject::GetTransform() const noexcept
{
	return m_transform;
}

inline bool GameObject::HasMaterial() const noexcept
{
	return (m_pMaterial != nullptr);
}

inline bool GameObject::HasDecal() const noexcept
{
	return (m_pDecal != nullptr);
}

inline bool GameObject::HasMesh() const noexcept
{
	return (m_pMesh != nullptr);
}