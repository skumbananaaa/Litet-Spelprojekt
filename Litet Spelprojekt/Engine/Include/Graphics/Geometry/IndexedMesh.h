#pragma once
#include <EnginePch.h>
#include <IO/IResource.h>
#include "MeshBase.h"

class API IndexedMesh : public IResource
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:
	void SetInstances(const InstanceData* const pInstances, uint32 numInstances) const noexcept;

	uint32 GetIndexCount() const noexcept;
	uint32 GetVertexCount() const noexcept;
	uint32 GetInstanceCount() const noexcept;

private:
	IndexedMesh(IndexedMesh&& other) = delete;
	IndexedMesh(const IndexedMesh& other) = delete;
	IndexedMesh& operator=(IndexedMesh&& other) = delete;
	IndexedMesh& operator=(const IndexedMesh& other) = delete;

	IndexedMesh(const BaseVertex* const pVertices, const uint32* const pIndices, uint32 numVertices, uint32 numIndices) noexcept;
	~IndexedMesh();

	virtual void Construct() override;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_InstanceBuffer;
	uint32 m_VertexCount;
	uint32 m_IndexCount;
	const BaseVertex* m_pVertices;
	const uint32* m_pIndices;
	mutable uint32 m_NumInstances;
	mutable uint32 m_NumReservedInstances;

private:
	static IndexedMesh* CreateIndexedMeshFromFile(const char* pFilename);
	static IndexedMesh* CreateCube();
	static IndexedMesh* CreateCubeInvNormals();
	static IndexedMesh* CreateQuad();
};

inline uint32 IndexedMesh::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline uint32 IndexedMesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}

inline uint32 IndexedMesh::GetInstanceCount() const noexcept
{
	return m_NumInstances;
}