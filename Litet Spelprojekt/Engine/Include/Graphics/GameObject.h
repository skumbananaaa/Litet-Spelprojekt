#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Geometry/AnimatedMesh.h>
#include <Graphics/Materials/AnimatedSkeleton.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/Decal.h>
#include <IO/ResourceHandler.h>
#include "Camera.h"

class Scene;

class API GameObject
{
public:
	GameObject() noexcept;
	virtual ~GameObject();

	virtual void Update(const Camera& camera, float deltaTime) noexcept;
	virtual void UpdateTransform() noexcept;
	
	virtual void SetName(const std::string& name) noexcept;
	virtual void SetIsReflectable(bool isReflectable) noexcept;
	virtual void SetIsVisible(bool isVisible) noexcept;
	virtual void SetMesh(int32 mesh) noexcept;
	virtual void SetAnimatedMesh(int32 mesh) noexcept;
	virtual void SetMaterial(int32 material) noexcept;
	virtual void SetDecal(int32 decal) noexcept;
	virtual void SetPosition(const glm::vec3& position) noexcept;
	virtual void SetRotation(const glm::vec4& rotation) noexcept;
	virtual void SetScale(const glm::vec3& scale) noexcept;
	virtual void SetRoom(uint32 room) noexcept;
	void SetHidden(bool isHidden) noexcept;

	const std::string& GetName() const noexcept;
	const Decal* GetDecal() const noexcept;
	const Material* GetMaterial() const noexcept;
	const IndexedMesh* GetMesh() const noexcept;
	const AnimatedMesh* GetAnimatedMesh() const noexcept;
	const AnimatedSkeleton* GetSkeleton() const noexcept;
	const glm::mat4& GetTransform() const noexcept;
	const glm::mat4& GetInverseTransform() const noexcept;
	const glm::vec3& GetPosition() const noexcept;
	const glm::vec4& GetRotation() const noexcept;
	const glm::vec3& GetScale() const noexcept;
	uint32 GetRoom() const noexcept;
	bool IsHidden() const noexcept;

	bool IsReflectable() const noexcept;
	bool IsVisible() const noexcept;
	bool HasMaterial() const noexcept;
	bool HasDecal() const noexcept;
	bool HasMesh() const noexcept;
	bool HasAnimatedMesh() const noexcept;
	bool HasSkeleton() const noexcept;

	void SetTypeId(int32 typeId) noexcept;
	int32 GetTypeId() const noexcept;

	virtual void OnAddedToScene(Scene* scene) noexcept {};
	virtual void OnFireDetected() noexcept {};
	virtual void OnSmokeDetected() noexcept {};
	virtual void OnWaterDetected() noexcept {};

protected:
	bool m_IsDirty;
	bool m_IsVisible;
	glm::mat4 m_transform;
	glm::mat4 m_InverseTransform;

private:
	std::string m_Name;
	const IndexedMesh* m_pMesh;
	const AnimatedMesh* m_pAMesh;
	const Material* m_pMaterial;
	const Decal* m_pDecal;
	AnimatedSkeleton* m_pASkeleton;
	glm::vec3 m_Position;
	glm::vec4 m_Rotation;
	glm::vec3 m_Scale;
	float m_ExtendPosX;
	bool m_IsReflectable;
	int32 m_TypeId;
	int32 m_Room = 1;
	bool m_IsHidden = false;
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

inline bool GameObject::IsHidden() const noexcept
{
	return m_IsHidden;
}

inline void GameObject::SetMesh(int32 mesh) noexcept
{
	m_pMesh = ResourceHandler::GetMesh(mesh);
}

inline void GameObject::SetAnimatedMesh(int32 mesh) noexcept
{
	m_pAMesh = ResourceHandler::GetAnimatedMesh(mesh);
	m_pASkeleton = new AnimatedSkeleton();
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

inline const AnimatedMesh* GameObject::GetAnimatedMesh() const noexcept
{
	assert(m_pAMesh != nullptr);
	return m_pAMesh;
}

inline const AnimatedSkeleton* GameObject::GetSkeleton() const noexcept
{
	assert(m_pASkeleton != nullptr);
	return m_pASkeleton;
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

inline bool GameObject::HasAnimatedMesh() const noexcept
{
	return (m_pAMesh != nullptr);
}

inline bool GameObject::HasSkeleton() const noexcept
{
	return (m_pASkeleton != nullptr);
}

inline void GameObject::SetTypeId(int32 typeId) noexcept
{
	m_TypeId = typeId;
}

inline int32 GameObject::GetTypeId() const noexcept
{
	return m_TypeId;
}

inline bool GameObject::IsDirty() const noexcept
{
	return m_IsDirty;
}
