#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/Joint.h>

struct AnimatedVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texCoords;
	glm::vec3 boneWeight;
	glm::ivec3 jointID;

	inline bool operator==(const AnimatedVertex& rs) const
	{
		return (position == rs.position) && (normal == rs.normal) && (tangent == rs.tangent) && (texCoords == rs.texCoords) && (jointID == rs.jointID) && (boneWeight == rs.boneWeight);
	}
};

struct AnimatedInstanceData
{
	glm::mat4 Model;
	glm::mat4 InverseModel;
	//glm::mat4 JointTransformation;
	glm::vec3 Direction;
};

class API AnimatedMesh
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:
	AnimatedMesh(AnimatedMesh&& other) = delete;
	AnimatedMesh(const AnimatedMesh& other) = delete;
	AnimatedMesh& operator=(AnimatedMesh&& other) = delete;
	AnimatedMesh& operator=(const AnimatedMesh& other) = delete;

	AnimatedMesh(const AnimatedVertex* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices, const Joint& rootJoint, uint32 jointCount) noexcept;
	~AnimatedMesh();

	void SetAnimatedInstances(const AnimatedInstanceData* const pInstances, uint32 numInstances) const noexcept;

	uint32 GetIndexCount() const noexcept;
	uint32 GetVertexCount() const noexcept;
	uint32 GetInstanceCount() const noexcept;
	uint32 GetJointCount() const noexcept;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_InstanceBuffer;
	uint32 m_VertexCount;
	uint32 m_IndexCount;
	uint32 m_JointCount;

	Joint* m_pRootJoint;
	//Animator* m_pAnimator;



	mutable uint32 m_NumInstances;
	mutable uint32 m_NumReservedInstances;

private:
	static AnimatedMesh* CreateAnimatedMeshFromFile(const char* pFilename);

};


inline uint32 AnimatedMesh::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline uint32 AnimatedMesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}

inline uint32 AnimatedMesh::GetInstanceCount() const noexcept
{
	return m_NumInstances;
}

inline uint32 AnimatedMesh::GetJointCount() const noexcept
{
	return m_JointCount;
}

