#include <EnginePch.h>
#include <Graphics/Geometry/AnimatedMesh.h>
#include <Graphics/Renderers/GLContext.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AnimatedMesh::AnimatedMesh(const AnimatedVertex* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept
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

	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glGenBuffers(1, &m_VBO));
	GL_CALL(glGenBuffers(1, &m_IBO));
	GL_CALL(glGenBuffers(1, &m_InstanceBuffer));

	GL_CALL(glBindVertexArray(m_VAO));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(AnimatedVertex), vertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), indices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//Normal

	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(AnimatedVertex), (void*)(3 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(1));
	//Tangent
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(AnimatedVertex), (void*)(6 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(2));
	//TexCoords
	GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)(9 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(3));

	m_NumReservedInstances = 10;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumReservedInstances * sizeof(InstanceData), nullptr, GL_STATIC_DRAW));

	//Instance model matrix
	GL_CALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0));
	GL_CALL(glEnableVertexAttribArray(4));
	GL_CALL(glVertexAttribDivisor(4, 1));
	GL_CALL(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(5));
	GL_CALL(glVertexAttribDivisor(5, 1));
	GL_CALL(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(2 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(6));
	GL_CALL(glVertexAttribDivisor(6, 1));
	GL_CALL(glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(3 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(7));
	GL_CALL(glVertexAttribDivisor(7, 1));

	//Instance inversemodel matrix
	GL_CALL(glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(4 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(8));
	GL_CALL(glVertexAttribDivisor(8, 1));
	GL_CALL(glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(5 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(9));
	GL_CALL(glVertexAttribDivisor(9, 1));
	GL_CALL(glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(6 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(10));
	GL_CALL(glVertexAttribDivisor(10, 1));
	GL_CALL(glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(7 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(11));
	GL_CALL(glVertexAttribDivisor(11, 1));

	//Instance direction matrix
	GL_CALL(glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(8 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(12));
	GL_CALL(glVertexAttribDivisor(12, 1));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

AnimatedMesh::~AnimatedMesh()
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

void AnimatedMesh::SetInstances(const AnimatedInstanceData* const pInstances, uint32 numInstances) const noexcept
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));

	uint32 size = sizeof(AnimatedInstanceData) * numInstances;
	if (numInstances > m_NumReservedInstances)
	{
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		m_NumReservedInstances = numInstances;
	}

	GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, pInstances));
	m_NumInstances = numInstances;

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

AnimatedMesh* AnimatedMesh::CreateAnimatedMeshFromFile(const char* pFilename)
{

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(pFilename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr || pScene->mNumAnimations == 0)
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
	std::vector<AnimatedVertex> vertices(pMesh->mNumVertices);
	for (uint32 i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices[i].position = glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		vertices[i].normal = (pMesh->HasNormals()) ? glm::vec3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z) : glm::vec3();
		vertices[i].tangent = (pMesh->HasTangentsAndBitangents()) ? glm::vec3(pMesh->mTangents[i].x, pMesh->mTangents[i].y, pMesh->mTangents[i].z) : glm::vec3();
		vertices[i].texCoords = (pMesh->HasTextureCoords(0)) ? glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y) : glm::vec2();
		vertices[i].boneInfo1 = glm::vec4();
		vertices[i].boneInfo2 = glm::vec4();
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

	return new AnimatedMesh(vertices.data(), indices.data(), static_cast<uint32>(vertices.size()), static_cast<uint32>(indices.size()));
}
