#pragma once

#include <EnginePch.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/Decal.h>
#include <IO/ResourceHandler.h>

class API GameObject
{
public:
	GameObject() noexcept;
	virtual ~GameObject();

	virtual void SetIsReflectable(bool isReflectable) noexcept;
	virtual void SetMesh(uint32 mesh) noexcept;
	virtual void SetMaterial(uint32 material) noexcept;
	virtual void SetDecal(uint32 decal) noexcept;
	virtual void SetPosition(const glm::vec3& position) noexcept;
	virtual void SetRotation(const glm::vec4& rotation) noexcept;
	virtual void SetScale(const glm::vec3& scale) noexcept;

	const Decal* GetDecal() const noexcept;
	const Material* GetMaterial() const noexcept;
	const IndexedMesh* GetMesh() const noexcept;
	const glm::mat4& GetTransform() const noexcept;
	const glm::mat4& GetInverseTransform() const noexcept;
	const glm::vec3& GetPosition() const noexcept;

	bool IsReflectable() const noexcept;
	bool HasMaterial() const noexcept;
	bool HasDecal() const noexcept;
	bool HasMesh() const noexcept;

	virtual void UpdateTransform() noexcept;

private:
	const IndexedMesh* m_pMesh;
	const Material* m_pMaterial;
	const Decal* m_pDecal;
	glm::vec3 m_Position;
	glm::vec4 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_transform;
	glm::mat4 m_InverseTransform;
	bool m_IsDirty;
	bool m_IsReflectable;
};

inline const glm::vec3& GameObject::GetPosition() const noexcept
{
	return m_Position;
}

inline void GameObject::SetMesh(uint32 mesh) noexcept
{
	m_pMesh = ResourceHandler::GetMesh(mesh);
}

inline void GameObject::SetDecal(uint32 decal) noexcept
{
	m_pDecal = ResourceHandler::GetDecal(decal);
}

inline void GameObject::SetMaterial(uint32 material) noexcept
{
	m_pMaterial = ResourceHandler::GetMaterial(material);
}

inline const Decal* GameObject::GetDecal() const noexcept
{
	assert(m_pDecal != nullptr);
	return m_pDecal;
}

inline const Material* GameObject::GetMaterial() const noexcept
{
	assert(m_pMaterial != nullptr);
	return m_pMaterial;
}

inline const IndexedMesh* GameObject::GetMesh() const noexcept
{
	assert(m_pMesh != nullptr);
	return m_pMesh;
}

inline const glm::mat4& GameObject::GetTransform() const noexcept
{
	return m_transform;
}

inline const glm::mat4& GameObject::GetInverseTransform() const noexcept
{
	return m_InverseTransform;
}

inline bool GameObject::IsReflectable() const noexcept
{
	return m_IsReflectable;
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