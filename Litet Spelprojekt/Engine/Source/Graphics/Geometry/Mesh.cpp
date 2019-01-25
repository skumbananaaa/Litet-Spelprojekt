#include <EnginePch.h>
#include <Graphics/Geometry/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(const Vertex* const vertices, uint32 numVertices) noexcept
{
	m_VertexCount = numVertices;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

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

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

Mesh* Mesh::CreateMeshFromFile(const char* pFilename)
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

	return new Mesh(vertices.data(), static_cast<unsigned int>(vertices.size()));
}

Mesh* Mesh::CreateCube()
{
	Vertex triangleVertices[] =
	{
		// Front (Seen from front)
		{ glm::vec3(-1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F,  1.0F),	glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Top (Seen from above)
		{ glm::vec3(-1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F,  1.0F,  1.0F),	glm::vec3(0.0F,  1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Back (Seen from front)
		{ glm::vec3(-1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F, -1.0F),	glm::vec3(0.0F,  0.0F, -1.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Bottom (Seen from above)
		{ glm::vec3(-1.0F, -1.0F, -1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F, -1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F,  1.0F),	glm::vec3(0.0F, -1.0F,  0.0F),	 glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) },

		// Left (Seen from left)
		{ glm::vec3(-1.0F,  1.0F, -1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(-1.0F,  1.0F,  1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(-1.0F, -1.0F,  1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-1.0F, -1.0F, -1.0F),	glm::vec3(-1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 0.0F) },

		// Right (Seen from left)
		{ glm::vec3(1.0F,  1.0F, -1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(1.0F,  1.0F,  1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(1.0F, -1.0F,  1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(1.0F, -1.0F, -1.0F),	glm::vec3(1.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  1.0F),	 glm::vec2(0.0F, 0.0F) }
	};

	return new Mesh(triangleVertices, 24);
}

Mesh* Mesh::CreateGrid(int32 width, int32 height, int32 depth)
{
	int nrOfVertices = ((width + 1) * (height + 1) + (height + 1) * (depth + 1) + (width + 1) * (depth + 1)) * 2;
	Vertex* pVertices = new Vertex[nrOfVertices];
	int counter = 0;

	for (int y = -height / 2; y <= height / 2; y++)
	{
		for (int x = -width / 2; x <= width / 2; x++)
		{
			pVertices[counter++] = { glm::vec3(x, y, -depth / 2),	glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) };
			pVertices[counter++] = { glm::vec3(x, y, depth / 2), glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) };
		}
	}

	for (int y = -height / 2; y <= height / 2; y++)
	{
		for (int z = -depth / 2; z <= depth / 2; z++)
		{
			pVertices[counter++] = { glm::vec3(-width / 2, y, z), glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) };
			pVertices[counter++] = { glm::vec3(width / 2, y, z), glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) };
		}
	}

	for (int x = -width / 2; x <= width / 2; x++)
	{
		for (int z = -depth / 2; z <= depth / 2; z++)
		{
			pVertices[counter++] = { glm::vec3(x, -height / 2, z), glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) };
			pVertices[counter++] = { glm::vec3(x, height / 2, z), glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec3(0.0F,  0.0F,  0.0F),	 glm::vec2(0.0F, 0.0F) };
		}
	}

	Mesh* pMesh = new Mesh(pVertices, counter);
	delete[] pVertices;

	return pMesh;
}
