#pragma once
#include <EnginePch.h>
#include <IO/IResource.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <map>

struct VertexMeshData
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texCoords;
};

struct VertexBoneData
{
	glm::ivec4 IDs = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);

	void AddBoneData(uint32 BoneID, float Weight);
};

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
		VertexMeshData* vertices,
		uint32* indices,
		VertexBoneData* bones,
		uint32 numVertices,
		uint32 numIndices,
		uint32 numBones,
		glm::mat4& globalTransform,
		std::map<std::string, uint32>& boneMap,
		std::vector<glm::mat4>& boneOffsets,
		const void* rootNode,
		const void* scene,
		const void* importer,
		std::vector<MeshEntry>& entries) noexcept;

	~AnimatedMesh();

private:
	virtual void Construct() override;
	static AnimatedMesh* ReadColladaFile(const char* daeFile);

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

	VertexMeshData* m_Vertices;
	uint32 m_NumVerts;

	VertexBoneData* m_Bones;
	uint32 m_NumBones;

	uint32* m_Indices;
	uint32 m_NumIndices;


	mutable std::map<std::string, uint32> m_BoneMap;
	std::vector<MeshEntry> m_Entries;
	mutable std::vector<glm::mat4> m_BoneOffsets;
};