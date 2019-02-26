#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Renderers/GLContext.h>

IndexedMesh::IndexedMesh(const void* const pVertices, const uint32* const pIndices, uint32 numVertices, uint32 numIndices) noexcept
	: m_VAO(0),
	m_VBO(0),
	m_IBO(0),
	m_InstanceBuffer(0),
	m_VertexCount(0),
	m_IndexCount(0),
	m_NumInstances(0),
	m_NumReservedInstances(0)
{
	m_VertexCount = numVertices;
	m_IndexCount = numIndices;
	m_pVertices = const_cast<void*>(pVertices);
	m_pIndices = const_cast<uint32*>(pIndices);
}

IndexedMesh::~IndexedMesh()
{
	if (glIsVertexArray(m_VAO))
	{
		GL_CALL(glDeleteVertexArrays(1, &m_VAO));
		m_VAO = 0;
	}

	if (glIsBuffer(m_VBO))
	{
		GL_CALL(glDeleteBuffers(1, &m_VBO));
		m_VBO = 0;
	}

	if (glIsBuffer(m_IBO))
	{
		GL_CALL(glDeleteBuffers(1, &m_IBO));
		m_IBO = 0;
	}

	if (glIsBuffer(m_InstanceBuffer))
	{
		GL_CALL(glDeleteBuffers(1, &m_InstanceBuffer));
		m_InstanceBuffer = 0;
	}
}

void IndexedMesh::Construct()
{
	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glGenBuffers(1, &m_VBO));
	GL_CALL(glGenBuffers(1, &m_IBO));
	GL_CALL(glGenBuffers(1, &m_InstanceBuffer));

	GL_CALL(glBindVertexArray(m_VAO));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(BaseVertex), m_pVertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), m_pIndices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BaseVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//Normal
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(BaseVertex), (void*)(3 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(1));
	//TexCoords
	GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVertex), (void*)(6 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(2));

	m_NumReservedInstances = 10;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumReservedInstances * sizeof(InstanceData), nullptr, GL_STATIC_DRAW));

	//Instance model matrix
	GL_CALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0));
	GL_CALL(glEnableVertexAttribArray(3));
	GL_CALL(glVertexAttribDivisor(3, 1));
	GL_CALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(4));
	GL_CALL(glVertexAttribDivisor(4, 1));
	GL_CALL(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(2 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(5));
	GL_CALL(glVertexAttribDivisor(5, 1));
	GL_CALL(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(3 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(6));
	GL_CALL(glVertexAttribDivisor(6, 1));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	free(m_pVertices);
	m_pVertices = nullptr;
	DeleteArrSafe(m_pIndices);
}

void IndexedMesh::SetInstances(const InstanceData* const pInstances, uint32 numInstances) const noexcept
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
	
	uint32 size = sizeof(InstanceData) * numInstances;
	if (numInstances > m_NumReservedInstances)
	{
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		m_NumReservedInstances = numInstances;
	}
	
	GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, pInstances));
	m_NumInstances = numInstances;

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexedMesh* IndexedMesh::CreateIndexedMeshFromFile(const char* pFilename)
{
	Assimp::Importer importer;
	const aiScene* pScene = nullptr;
#if !defined(_DEBUG)
	pScene = importer.ReadFile(pFilename, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_DropNormals | aiProcess_GenNormals | aiProcess_ImproveCacheLocality | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
#else
	pScene = importer.ReadFile(pFilename, aiProcess_DropNormals | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
#endif
	if (pScene == nullptr)
	{
		std::cout << "Failed to load mesh '" << pFilename << "'. Error: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	if (pScene->mNumMeshes < 1)
	{
		std::cout << "File '" << pFilename << "' does not contain any meshes." << std::endl;
		return nullptr;
	}

	const aiMesh* pMesh = pScene->mMeshes[0];
	BaseVertex* pVertices = new BaseVertex[pMesh->mNumVertices];
	for (uint32 i = 0; i < pMesh->mNumVertices; i++)
	{
		pVertices[i].Position = glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		pVertices[i].Normal = (pMesh->HasNormals()) ? glm::vec3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z) : glm::vec3();
		pVertices[i].TexCoord = (pMesh->HasTextureCoords(0)) ? glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y) : glm::vec2();
	}

	uint32* pIndices = new uint32[pMesh->mNumFaces * 3];
	int32 index = 0;
	for (uint32 i = 0; i < pMesh->mNumFaces; i++)
	{
		const aiFace& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		pIndices[index++] = face.mIndices[0];
		pIndices[index++] = face.mIndices[1];
		pIndices[index++] = face.mIndices[2];
	}

	return new IndexedMesh(pVertices, pIndices, static_cast<uint32>(pMesh->mNumVertices), static_cast<uint32>(pMesh->mNumFaces * 3));
}

IndexedMesh* IndexedMesh::CreateCube()
{
	BaseVertex* pVertices = new BaseVertex[24]
	{
		// Front (Seen from front)
		BaseVertex( glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(0.0F, 0.0F) ),

		// Top (Seen from above)
		BaseVertex( glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(0.0F, 0.0F) ),

		// Back (Seen from front)
		BaseVertex( glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(0.0F, 0.0F) ),

		// Bottom (Seen from above)
		BaseVertex( glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(0.0F, 0.0F) ),

		// Left (Seen from left)
		BaseVertex( glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 0.0F) ),

		// Right (Seen from left)
		BaseVertex( glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 0.0F) )
	};

	uint32* pIndices = new uint32[36]
	{
		// Front (Seen from front)
		0, 2, 1,
		2, 0, 3,

		// Top (Seen from above)
		4, 6, 5,
		6, 4, 7,

		// Back (Seen from front)
		8, 9, 10,
		10, 11, 8,

		// Bottom (Seen from above)
		12, 13, 14,
		14, 15, 12,

		// Left (Seen from left)
		16, 18, 17,
		18, 16, 19,

		// Right (Seen from left)
		20, 21, 22,
		22, 23, 20
	};

	return new IndexedMesh(pVertices, pIndices, 24, 36);
}

IndexedMesh* IndexedMesh::CreateCubeInvNormals()
{
	BaseVertex* pVertices = new BaseVertex[24]
	{
		// Front (Seen from front)
		BaseVertex( glm::vec3(-0.5F,  0.5F,  0.5F),	-glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F,  0.5F),	-glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F,  0.5F),	-glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F,  0.5F),	-glm::vec3(0.0F,  0.0F,  1.0F),	glm::vec2(0.0F, 0.0F) ),

		// Top (Seen from above)
		BaseVertex( glm::vec3(-0.5F,  0.5F, -0.5F),	-glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F, -0.5F),	-glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F,  0.5F),	-glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F,  0.5F,  0.5F),	-glm::vec3(0.0F,  1.0F,  0.0F),	glm::vec2(0.0F, 0.0F) ),

		// Back (Seen from front)
		BaseVertex( glm::vec3(-0.5F,  0.5F, -0.5F),	-glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F, -0.5F),	-glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F, -0.5F),	-glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F, -0.5F),	-glm::vec3(0.0F,  0.0F, -1.0F),	glm::vec2(0.0F, 0.0F) ),

		// Bottom (Seen from above)
		BaseVertex( glm::vec3(-0.5F, -0.5F, -0.5F),	-glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F, -0.5F),	-glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F,  0.5F),	-glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F,  0.5F),	-glm::vec3(0.0F, -1.0F,  0.0F),	glm::vec2(0.0F, 0.0F) ),

		// Left (Seen from left)
		BaseVertex( glm::vec3(-0.5F,  0.5F, -0.5F),	-glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(-0.5F,  0.5F,  0.5F),	-glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F,  0.5F),	-glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(-0.5F, -0.5F, -0.5F),	-glm::vec3(-1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 0.0F) ),

		// Right (Seen from left)
		BaseVertex( glm::vec3(0.5F,  0.5F, -0.5F),	-glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F,  0.5F,  0.5F),	-glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 1.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F,  0.5F),	-glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(1.0F, 0.0F) ),
		BaseVertex( glm::vec3(0.5F, -0.5F, -0.5F),	-glm::vec3(1.0F,  0.0F,  0.0F),	glm::vec2(0.0F, 0.0F) )
	};

	uint32* pIndices = new uint32[36]
	{
		// Front (Seen from front)
		0, 2, 1,
		2, 0, 3,

		// Top (Seen from above)
		4, 6, 5,
		6, 4, 7,

		// Back (Seen from front)
		8, 9, 10,
		10, 11, 8,

		// Bottom (Seen from above)
		12, 13, 14,
		14, 15, 12,

		// Left (Seen from left)
		16, 18, 17,
		18, 16, 19,

		// Right (Seen from left)
		20, 21, 22,
		22, 23, 20
	};

	return new IndexedMesh(pVertices, pIndices, 24, 36);
}

IndexedMesh* IndexedMesh::CreateQuad()
{
	BaseVertex* pVertices = new BaseVertex[4]
	{
		BaseVertex(glm::vec3(-0.5F,  0.5F,  0.0F), glm::vec3(0.0F,  0.0F,  1.0F), glm::vec2(0.0F, 1.0F)),
		BaseVertex(glm::vec3(0.5F,  0.5F,  0.0F),  glm::vec3(0.0F,  0.0F,  1.0F), glm::vec2(1.0F, 1.0F)),
		BaseVertex(glm::vec3(0.5F, -0.5F,  0.0F),  glm::vec3(0.0F,  0.0F,  1.0F), glm::vec2(1.0F, 0.0F)),
		BaseVertex(glm::vec3(-0.5F, -0.5F,  0.0F), glm::vec3(0.0F,  0.0F,  1.0F), glm::vec2(0.0F, 0.0F))
	};

	uint32* pIndices = new uint32[6]
	{
		// Front (Seen from front)
		0, 2, 1,
		2, 0, 3
	};

	return new IndexedMesh(pVertices, pIndices, 4, 6);
}
