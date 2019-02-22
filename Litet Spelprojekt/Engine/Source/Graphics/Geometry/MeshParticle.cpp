#include <EnginePch.h>
#include <Graphics/Geometry/MeshParticle.h>
#include <Graphics/Renderers/GLContext.h>

MeshParticle::MeshParticle(const MeshParticleVertex* const pVertices, const uint32* const pIndices, uint32 numVertices, uint32 numIndices) noexcept
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
	m_pVertices = pVertices;
	m_pIndices = pIndices;
}

MeshParticle::~MeshParticle()
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

void MeshParticle::Construct()
{
	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glGenBuffers(1, &m_VBO));
	GL_CALL(glGenBuffers(1, &m_IBO));
	GL_CALL(glGenBuffers(1, &m_InstanceBuffer));

	GL_CALL(glBindVertexArray(m_VAO));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(MeshParticleVertex), m_pVertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), m_pIndices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshParticleVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//TexCoords
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshParticleVertex), (void*)(3 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(1));

	m_NumReservedInstances = 10;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumReservedInstances * sizeof(ParticleInstance), nullptr, GL_STATIC_DRAW));

	//Per instance position
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), (void*)0);
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribDivisor(2, 1));
	//Per instance color
	GL_CALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), (void*)sizeof(glm::vec3))));
	GL_CALL(glEnableVertexAttribArray(3));
	GL_CALL(glVertexAttribDivisor(3, 1));
	//Per instance scale
	GL_CALL(glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), (void*)(sizeof(float) * 7)));
	GL_CALL(glEnableVertexAttribArray(4));
	GL_CALL(glVertexAttribDivisor(4, 1));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	DeleteArrSafe(m_pVertices);
	DeleteArrSafe(m_pIndices);
}

void MeshParticle::SetInstances(const ParticleInstance* const pInstances, uint32 numInstances) const noexcept
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));

	uint32 size = sizeof(ParticleInstance) * numInstances;
	if (numInstances > m_NumReservedInstances)
	{
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		m_NumReservedInstances = numInstances;
	}

	GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, pInstances));
	m_NumInstances = numInstances;

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

MeshParticle* MeshParticle::CreateMeshParticleFromFile(const char* pFilename)
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
	MeshParticleVertex* vertices = new MeshParticleVertex[pMesh->mNumVertices];
	for (uint32 i = 0; i < pMesh->mNumVertices; i++)
	{
		vertices[i].Position = glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
		vertices[i].TexCoords = (pMesh->HasTextureCoords(0)) ? glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y) : glm::vec2();
	}

	uint32* indices = new uint32[pMesh->mNumFaces * 3];
	int32 index = 0;
	for (uint32 i = 0; i < pMesh->mNumFaces; i++)
	{
		const aiFace& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		indices[index++] = face.mIndices[0];
		indices[index++] = face.mIndices[1];
		indices[index++] = face.mIndices[2];
	}

	return new MeshParticle(vertices, indices, static_cast<uint32>(pMesh->mNumVertices), static_cast<uint32>(pMesh->mNumFaces * 3));
}

MeshParticle* MeshParticle::CreateCube()
{
	MeshParticleVertex* pVertices = new MeshParticleVertex[24]
	{
		// Front (Seen from front)
		{ glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec2(0.0F, 0.0F) },

		// Top (Seen from above)
		{ glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec2(0.0F, 0.0F) },

		// Back (Seen from front)
		{ glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec2(0.0F, 0.0F) },

		// Bottom (Seen from above)
		{ glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec2(0.0F, 0.0F) },

		// Left (Seen from left)
		{ glm::vec3(-0.5F,  0.5F, -0.5F),	glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(-0.5F,  0.5F,  0.5F),	glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(-0.5F, -0.5F,  0.5F),	glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(-0.5F, -0.5F, -0.5F),	glm::vec2(0.0F, 0.0F) },

		// Right (Seen from left)
		{ glm::vec3(0.5F,  0.5F, -0.5F),	glm::vec2(0.0F, 1.0F) },
		{ glm::vec3(0.5F,  0.5F,  0.5F),	glm::vec2(1.0F, 1.0F) },
		{ glm::vec3(0.5F, -0.5F,  0.5F),	glm::vec2(1.0F, 0.0F) },
		{ glm::vec3(0.5F, -0.5F, -0.5F),	glm::vec2(0.0F, 0.0F) },
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

	return new MeshParticle(pVertices, pIndices, 24, 36);
}