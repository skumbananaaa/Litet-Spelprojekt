#include <EnginePch.h>
#include <Graphics/Geometry/AnimatedMesh.h>
#include <Graphics/Renderers/GLContext.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Graphics/Geometry/AssimpHelper.inl>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>

AnimatedMesh::AnimatedMesh(VertexMeshData* vertices,
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
	std::vector<MeshEntry>& entries) noexcept
	: m_VAO(0),
	m_VBO(0),
	m_IBO(0),
	m_BonesBuffer(0),
	m_InstanceBuffer(0),
	m_NumVerts(0),
	m_NumIndices(0),
	m_GlobalInverseTransform(0.0f),
	m_Vertices(nullptr),
	m_Bones(nullptr),
	m_Indices(nullptr),
	m_pRootNode(nullptr),
	m_pScene(nullptr),
	m_NumBones(0)
{
	m_Vertices = new VertexMeshData[numVertices];
	memcpy(m_Vertices, vertices, sizeof(VertexMeshData) * numVertices);

	m_Indices = new uint32[numIndices];
	memcpy(m_Indices, indices, sizeof(uint32) * numIndices);

	m_Bones = new VertexBoneData[numVertices];
	memcpy(m_Bones, bones, sizeof(VertexBoneData) * numVertices);
	DeleteArrSafe(bones);

	m_pImporter = importer;

	m_NumVerts = numVertices;
	m_NumIndices = numIndices;
	m_NumBones = numBones;
	m_GlobalInverseTransform = globalTransform;
	m_BoneOffsets = boneOffsets;
	m_BoneMap = boneMap;
	m_pRootNode = rootNode;
	m_pScene = scene;
	m_Entries = entries;
}

AnimatedMesh::~AnimatedMesh()
{
	const Assimp::Importer* pImporter = reinterpret_cast<const Assimp::Importer*>(m_pImporter);
	DeleteSafe(pImporter);
}

AnimatedMesh* AnimatedMesh::ReadColladaFile(const char* daeFile)
{
	Assimp::Importer* importer = new Assimp::Importer();
	const aiScene* pScene;
	const aiNode* pRootNode;
	uint32 nrOfVerts = 0;
	uint32 nrOfbones = 0;
	uint32 nrOfIndices = 0;
	//VertexMeshData* pVerts = nullptr;
	VertexBoneData* pBones = nullptr;
	//uint32* pIndices = nullptr;
	glm::mat4 globalTransform(0.0);
	std::map<std::string, uint32> boneMap;
	std::vector<glm::mat4> boneOffsets;
	std::vector<MeshEntry> entries;


	pScene = importer->ReadFile(daeFile, aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_ConvertToLeftHanded);
	pRootNode = pScene->mRootNode;

	if (!pScene)
	{
		std::cout << "Dae file couldn't be loaded" << std::endl;
		return nullptr;
	}
	else
	{
		glm::mat4 globalTransform = AssimpToGLMMat4(pScene->mRootNode->mTransformation);
		globalTransform = glm::inverse(globalTransform);
		//InitFromScene(daeFile);

		const aiMesh* pMesh = pScene->mMeshes[0];
		const aiBone* paiBones = pMesh->mBones[0];

		std::vector<MeshEntry> entries;
		//m_Entries.resize(pScene->mNumMeshes);
		entries.resize(pScene->mNumMeshes);
		//std::cout << "number of Meshes in dae-file: " << m_Entries.size() << std::endl;
		std::cout << "number of Meshes in dae-file: " << entries.size() << std::endl;


		//making space for the vertex attributes and indices.
		std::vector<aiMesh*> paiMeshes;

		std::cout << "Loading dae-file data..." << std::endl;
		//counting the number of indices and verteices in mesh
		for (uint32 i = 0; i < entries.size(); i++)
		{
			entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			entries[i].BaseVertex = nrOfVerts;
			entries[i].BaseIndex = nrOfIndices;

			nrOfVerts += pScene->mMeshes[i]->mNumVertices;
			nrOfIndices += entries[i].NumIndices;

			//load all the meshes in to a vector array. 
			paiMeshes.push_back(pScene->mMeshes[i]);
			//NumIndices += m_Entries[i].NumIndices;
		}

		std::vector<VertexMeshData> verts;
		std::vector<uint32> indices;

		//pVerts = new VertexMeshData[nrOfVerts];
		pBones = new VertexBoneData[nrOfVerts];


		//Extracting all the needded data from the mesh/meshes
		for (uint32 i = 0; i < entries.size(); i++)
		{
			for (uint32 j = 0; j < paiMeshes.at(i)->mNumVertices; j++)
			{
				VertexMeshData data = {};
				data.position = glm::vec3(paiMeshes.at(i)->mVertices[j].x, paiMeshes.at(i)->mVertices[j].y, paiMeshes.at(i)->mVertices[j].z);
				data.normal = (paiMeshes.at(i)->HasNormals()) ? glm::vec3(paiMeshes.at(i)->mNormals[j].x, paiMeshes.at(i)->mNormals[j].y, paiMeshes.at(i)->mNormals[j].z) : glm::vec3();
				data.tangent = (paiMeshes.at(i)->HasTangentsAndBitangents()) ? glm::vec3(paiMeshes.at(i)->mTangents[j].x, paiMeshes.at(i)->mTangents[j].y, paiMeshes.at(i)->mTangents[j].z) : glm::vec3();
				data.texCoords = (paiMeshes.at(i)->HasTextureCoords(0)) ? glm::vec2(paiMeshes.at(i)->mTextureCoords[0][j].x, paiMeshes.at(i)->mTextureCoords[0][j].y) : glm::vec2();

				verts.push_back(data);
			}

			int32 index = 0;
			for (uint32 f = 0; f < paiMeshes.at(i)->mNumFaces; f++)
			{
				const aiFace& Face = paiMeshes.at(i)->mFaces[f];
				if (Face.mNumIndices == 3)
				{
					indices.push_back(Face.mIndices[0]);
					indices.push_back(Face.mIndices[1]);
					indices.push_back(Face.mIndices[2]);
				}
			}

			//loop through bones
			for (uint32 b = 0; b < paiMeshes.at(i)->mNumBones; b++)
			{
				uint32 boneIndex = 0;
				std::string boneName(paiMeshes.at(i)->mBones[b]->mName.data);
				std::cout << "extracting data from bone: " << paiMeshes.at(i)->mBones[b]->mName.data << std::endl;

				if (boneMap.find(boneName) == boneMap.end())
				{
					//allocate an index for a new bone
					glm::mat4 boneOffset;
					boneIndex = nrOfbones;
					nrOfbones++;
					boneOffsets.push_back(boneOffset);
					boneOffsets[boneIndex] = AssimpToGLMMat4(pMesh->mBones[b]->mOffsetMatrix); //function that transforms assimp matrix to glm matrix
					boneMap[boneName] = boneIndex;
				}
				else
				{
					boneIndex = boneMap[boneName];
				}

				//loop through weights
				for (uint32 w = 0; w < paiMeshes.at(i)->mBones[b]->mNumWeights; w++)
				{
					uint32 vertexID = entries[i].BaseVertex + paiMeshes.at(i)->mBones[b]->mWeights[w].mVertexId;
					float weight = paiMeshes.at(i)->mBones[b]->mWeights[w].mWeight;
					pBones[vertexID].AddBoneData(boneIndex, weight);

					//std::cout << vertexID << ", " <<  glm::to_string(pBones[vertexID].IDs) << ", " << glm::to_string(pBones[vertexID].BoneWeights) << std::endl;
				}
			}

			//////load bones and send them to VAO
		}

		return new AnimatedMesh(verts.data(), indices.data(), pBones, nrOfVerts, nrOfIndices, nrOfbones, globalTransform, boneMap, boneOffsets, pRootNode, pScene, importer, entries);
	}
}

void AnimatedMesh::Construct()
{

	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glGenBuffers(1, &m_VBO));
	GL_CALL(glGenBuffers(1, &m_IBO));
	GL_CALL(glGenBuffers(1, &m_BonesBuffer));

	GL_CALL(glBindVertexArray(m_VAO));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumVerts * sizeof(VertexMeshData), m_Vertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices * sizeof(uint32), m_Indices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexMeshData), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//Normal
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(VertexMeshData), (void*)(3 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(1));
	//Tangent
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(VertexMeshData), (void*)(6 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(2));
	//TexCoords
	GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexMeshData), (void*)(9 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(3));
	/*GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));*/

	//Bone data that will be sent to shader for calculations. 
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_BonesBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumVerts * sizeof(VertexBoneData), m_Bones, GL_STATIC_DRAW));

	//BoneID
	GL_CALL(glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (void*)0));
	GL_CALL(glEnableVertexAttribArray(4));
	//BoneWeight
	GL_CALL(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)16));
	GL_CALL(glEnableVertexAttribArray(5));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	DeleteArrSafe(m_Vertices);
	DeleteArrSafe(m_Indices);
	DeleteArrSafe(m_Bones);
}

void VertexBoneData::AddBoneData(uint32 BoneID, float Weight)
{
	for (uint32 i = 0; i < MAX_NUM_BONES_PER_VERT; i++)
	{
		if (BoneWeights[i] == 0.0f)
		{
			IDs[i] = BoneID;
			BoneWeights[i] = Weight;
			return;
		}
	}
}