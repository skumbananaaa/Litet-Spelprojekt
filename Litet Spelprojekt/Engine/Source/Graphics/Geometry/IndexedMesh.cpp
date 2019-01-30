#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

IndexedMesh::IndexedMesh(const Vertex* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept
{
	m_VertexCount = numVertices;
	m_IndexCount = numIndices;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); //Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(3 * sizeof(float))); //Normal 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(6 * sizeof(float))); //Tangent 
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float))); //TexCoords
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexedMesh::~IndexedMesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

IndexedMesh* IndexedMesh::CreateIndexedMeshFromFile(const char* pFilename)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(pFilename, aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
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
	std::vector<Vertex> vertices(pMesh->mNumVertices);
	for (uint32 i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices[i].position = glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		vertices[i].normal = (pMesh->HasNormals()) ? glm::vec3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z) : glm::vec3();
		vertices[i].texCoords = (pMesh->HasTextureCoords(0)) ? glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y) : glm::vec2();
		vertices[i].tangent = (pMesh->HasTangentsAndBitangents()) ? glm::vec3(pMesh->mTangents[i].x, pMesh->mTangents[i].y, pMesh->mTangents[i].z) : glm::vec3();
	}

	std::vector<uint32> indices;
	for (uint32 i = 0; i < pMesh->mNumFaces; i++)
	{
		const aiFace& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return new IndexedMesh(vertices.data(), indices.data(), static_cast<uint32>(vertices.size()), static_cast<uint32>(indices.size()));
}

IndexedMesh* IndexedMesh::CreateCube()
{
	Vertex triangleVertices[] =
	{
		// Front (Seen from front)
		{ glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Top (Seen from above)
		{ glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Back (Seen from front)
		{ glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Bottom (Seen from above)
		{ glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Left (Seen from left)
		{ glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 0.0F) },

		// Right (Seen from left)
		{ glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 0.0F) }
	};

	uint32 triangleIndices[] =
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

	return new IndexedMesh(triangleVertices, triangleIndices, 24, 36);
}

IndexedMesh* IndexedMesh::CreateQuad()
{
	Vertex vertices[]
	{
		{ glm::vec3(-1.0F,  1.0F,  0.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  0.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  0.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F,  0.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) }
	};

	uint32 indices[]
	{
		// Front (Seen from front)
		0, 2, 1,
		2, 0, 3
	};

	return new IndexedMesh(vertices, indices, 4, 6);
}
