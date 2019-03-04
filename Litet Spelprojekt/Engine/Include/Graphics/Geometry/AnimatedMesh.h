#pragma once
#include <EnginePch.h>
#include <IO/IResource.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include "MeshBase.h"

class API AnimatedMesh : public IResource
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;
	friend class AnimatedSkeleton;

private:
	struct MeshEntry
	{
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = 0xFFFFFFFF;
		}

		uint32 BaseVertex;
		uint32 BaseIndex;
		uint32 NumIndices;
		uint32 MaterialIndex;
	};

public:
	AnimatedMesh(
		BaseVertex* pVertices,
		uint32* pIndices,
		VertexBoneData* pBones,
		uint32 numVertices,
		uint32 numIndices,
		uint32 numBones,
		glm::mat4& globalTransform,
		std::map<std::string, uint32>& boneMap,
		std::vector<glm::mat4>& boneOffsets,
		const void* pRootNode,
		const void* pScene,
		const void* pImporter,
		std::vector<MeshEntry>& entries) noexcept;

	~AnimatedMesh();

private:
	virtual void Construct() override;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_BonesBuffer;
	uint32 m_InstanceBuffer;
	uint32 m_Texture;
	uint32 m_NumOfJoints;
	uint32 m_BoneLocation[MAX_NUM_BONES];
	const void* m_pScene;
	const void* m_pRootNode;
	const void* m_pImporter;
	glm::mat4 m_GlobalInverseTransform;
	BaseVertex* m_pVertices;
	uint32 m_NumVerts;
	VertexBoneData* m_pBones;
	uint32 m_NumBones;
	uint32* m_pIndices;
	uint32 m_NumIndices;
	std::vector<MeshEntry> m_Entries;
	mutable std::map<std::string, uint32> m_BoneMap;
	mutable std::vector<glm::mat4> m_BoneOffsets;
	
private:
	static AnimatedMesh* ReadColladaFile(const char* daeFile);
};