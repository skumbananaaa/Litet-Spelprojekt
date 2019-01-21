#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

IndexedMesh::IndexedMesh(const Vertex* const vertices, const unsigned int* const indices, unsigned int numVertices, unsigned int numIndices) noexcept
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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
	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices[i].position = glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		vertices[i].normal = (pMesh->HasNormals()) ? glm::vec3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z) : glm::vec3();
		vertices[i].texCoords = (pMesh->HasTextureCoords(0)) ? glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y) : glm::vec2();
		vertices[i].tangent = (pMesh->HasTangentsAndBitangents()) ? glm::vec3(pMesh->mTangents[i].x, pMesh->mTangents[i].y, pMesh->mTangents[i].z) : glm::vec3();
	}


	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		const aiFace& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return new IndexedMesh(vertices.data(), indices.data(), static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(indices.size()));
}

