#pragma once
#include <EnginePch.h>

class API Joint
{
private:
public:
	Joint(glm::mat4 bindLocalTransform, uint32 nrOfChildren = 0);
	~Joint();

	const glm::vec4& GetRotation() const noexcept;
	const glm::vec3& GetPosition() const noexcept;

	const glm::mat4& GetTransformMat() const noexcept;
	const glm::mat4& GetLocalBindTransform() const noexcept;
	const Joint* GetChild(uint32 index) const;
	const Joint* operator[](int index) const;

	const uint32& GetNrOfChildren() const noexcept;

	void SetRotation(const glm::vec4& rotationVec);
	void SetPosition(const glm::vec3& dir);
	void SetChild(uint32 index, const Joint * child) noexcept;
	void UpdateTransform(const glm::mat4 & parentTransform = glm::mat4(1.0f));

	static Joint* CreateFromJoint(const Joint * joint) noexcept;
private:
	uint32 m_NrOfChildren;
	Joint ** m_ppChildren;

	glm::mat4 m_Transform;
	glm::mat4 m_LocalBindTransform;
	glm::mat4 m_InverseBindTransform;

	glm::vec4 m_LocalRot;
	glm::vec3 m_LocalPos;
	bool m_isDirty;
};