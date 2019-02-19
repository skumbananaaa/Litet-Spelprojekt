#pragma once

#include <EnginePch.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/Decal.h>
#include <IO/ResourceHandler.h>

class Scene;

class API GameObject
{
public:
	GameObject() noexcept;
	virtual ~GameObject();

	virtual void SetName(const std::string& name) noexcept;
	virtual void SetIsReflectable(bool isReflectable) noexcept;
	virtual void SetIsVisible(bool isVisible) noexcept;
	virtual void SetMesh(int32 mesh) noexcept;
	virtual void SetMaterial(int32 material) noexcept;
	virtual void SetDecal(int32 decal) noexcept;
	virtual void SetPosition(const glm::vec3& position) noexcept;
	virtual void SetRotation(const glm::vec4& rotation) noexcept;
	virtual void SetScale(const glm::vec3& scale) noexcept;
	virtual void SetRoom(uint32 room) noexcept;
	void SetExtend(bool extend) noexcept;
	void Extend(float dtS) noexcept;
	void SetIsHidden(bool isCrew) noexcept;

	const std::string& GetName() const noexcept;
	const Decal* GetDecal() const noexcept;
	const Material* GetMaterial() const noexcept;
	const IndexedMesh* GetMesh() const noexcept;
	const glm::mat4& GetTransform() const noexcept;
	const glm::mat4& GetInverseTransform() const noexcept;
	const glm::vec3& GetPosition() const noexcept;
	const glm::vec4& GetRotation() const noexcept;
	const glm::vec3& GetScale() const noexcept;
	uint32 GetRoom() const noexcept;
	bool IsExtending() const noexcept;
	bool IsExtended() const noexcept;
	bool IsHidden() const noexcept;

	bool IsReflectable() const noexcept;
	bool IsVisible() const noexcept;
	bool HasMaterial() const noexcept;
	bool HasDecal() const noexcept;
	bool HasMesh() const noexcept;

	void SetTypeId(int32 typeId) noexcept;
	int32 GetTypeId() const noexcept;

	virtual void Update(float deltaTime) noexcept;
	virtual void UpdateTransform() noexcept;

	virtual void OnAddedToScene(Scene* scene) noexcept {};
	virtual void OnFireDetected() noexcept {};
	virtual void OnSmokeDetected() noexcept {};
	virtual void OnWaterDetected() noexcept {};

private:
	std::string m_Name;
	const IndexedMesh* m_pMesh;
	const Material* m_pMaterial;
	const Decal* m_pDecal;
	glm::vec3 m_Position;
	glm::vec4 m_Rotation;
	glm::vec3 m_Scale;
	glm::vec3 m_OriginalPos;
	float m_ExtendPosX;
	glm::mat4 m_transform;
	glm::mat4 m_InverseTransform;
	bool m_IsDirty;
	bool m_IsReflectable;
	bool m_IsVisible;
	int32 m_TypeId;
	int32 m_Room = 1;
	bool m_IsHidden = false;
	bool m_Extending = false;
	bool m_Extended = false;
};

inline const glm::vec3& GameObject::GetPosition() const noexcept
{
	return m_Position;
}

inline const glm::vec4& GameObject::GetRotation() const noexcept
{
	return m_Rotation;
}

inline const glm::vec3& GameObject::GetScale() const noexcept
{
	return m_Scale;
}

inline uint32 GameObject::GetRoom() const noexcept
{
	return m_Room;
}

inline bool GameObject::IsExtending() const noexcept
{
	return m_Extending;
}

inline bool GameObject::IsExtended() const noexcept
{
	return m_Extended;
}

inline bool GameObject::IsHidden() const noexcept
{
	return m_IsHidden;
}

inline void GameObject::SetMesh(int32 mesh) noexcept
{
	m_pMesh = ResourceHandler::GetMesh(mesh);
}

inline void GameObject::SetDecal(int32 decal) noexcept
{
	m_pDecal = ResourceHandler::GetDecal(decal);
}

inline void GameObject::SetMaterial(int32 material) noexcept
{
	m_pMaterial = ResourceHandler::GetMaterial(material);
}

inline const std::string& GameObject::GetName() const noexcept
{
	return m_Name;
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

inline bool GameObject::IsVisible() const noexcept
{
	return m_IsVisible;
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

inline void GameObject::SetTypeId(int32 typeId) noexcept
{
	m_TypeId = typeId;
}

inline int32 GameObject::GetTypeId() const noexcept
{
	return m_TypeId;
}
