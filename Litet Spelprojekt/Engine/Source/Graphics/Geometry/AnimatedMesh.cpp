#include <EnginePch.h>
#include <Graphics/Geometry/AnimatedMesh.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/Geometry/AssimpHelper.inl>

AnimatedMesh::AnimatedMesh(
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
		std::vector<MeshEntry>& entries) noexcept
	: m_VAO(0),
	m_VBO(0),
	m_IBO(0),
	m_BonesBuffer(0),
	m_InstanceBuffer(0),
	m_NumVerts(0),
	m_NumIndices(0),
	m_GlobalInverseTransform(0.0f),
	m_pVertices(nullptr),
	m_pBones(nullptr),
	m_pIndices(nullptr),
	m_pRootNode(nullptr),
	m_pScene(nullptr),
	m_NumBones(0)
{
	//Copy data for use in construct
	m_pVertices = new BaseVertex[numVertices];
	memcpy(m_pVertices, pVertices, sizeof(BaseVertex) * numVertices);
	m_pIndices = new uint32[numIndices];
	memcpy(m_pIndices, pIndices, sizeof(uint32) * numIndices);
	m_pBones = new VertexBoneData[numVertices];
	memcpy(m_pBones, pBones, sizeof(VertexBoneData) * numVertices);

	m_pImporter = pImporter;
	m_NumVerts = numVertices;
	m_NumIndices = numIndices;
	m_NumBones = numBones;
	m_GlobalInverseTransform = globalTransform;
	m_BoneOffsets = boneOffsets;
	m_BoneMap = boneMap;
	m_pRootNode = pRootNode;
	m_pScene = pScene;
	m_Entries = entries;
}

AnimatedMesh::~AnimatedMesh()
{
	const Assimp::Importer* pImporter = reinterpret_cast<const Assimp::Importer*>(m_pImporter);
	DeleteSafe(pImporter);
}

AnimatedMesh* AnimatedMesh::ReadColladaFile(const char* pFilename)
{
	Assimp::Importer* importer = new Assimp::Importer();
	const aiScene* pScene = nullptr;
	const aiNode* pRootNode = nullptr;
	VertexBoneData* pBones = nullptr;
	uint32 nrOfVerts = 0;
	uint32 nrOfbones = 0;
	uint32 nrOfIndices = 0;
	glm::mat4 globalTransform(0.0);
	std::map<std::string, uint32> boneMap;
	std::vector<glm::mat4> boneOffsets;
	std::vector<MeshEntry> entries;

#if !defined(_DEBUG)
	pScene = importer->ReadFile(pFilename, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_LimitBoneWeights | aiProcess_DropNormals | aiProcess_GenNormals | aiProcess_ImproveCacheLocality | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
#else
	pScene = importer->ReadFile(pFilename, aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_LimitBoneWeights | aiProcess_DropNormals | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);
#endif

	//pScene = importer->ReadFile(daeFile, aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_ConvertToLeftHanded);
	pRootNode = pScene->mRootNode;
	if (!pScene)
	{
		std::cout << "Dae file couldn't be loaded" << std::endl;
		return nullptr;
	}
	else
	{

		if (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			std::cout << "Error: Scene incomplete." << std::endl;
			return nullptr;
		}

		if (!pScene->HasMeshes())
		{
			std::cout << "Error: Scene does not contain any meshes." << std::endl;
			return nullptr;
		}
		else
		{
			std::cout << "Scene has " << pScene->mNumMeshes << " meshes." << std::endl;
		}

		glm::mat4 globalTransform = AssimpToGLMMat4(pScene->mRootNode->mTransformation);
		globalTransform = glm::inverse(globalTransform);

		const aiMesh* pMesh = pScene->mMeshes[0];
		if (!pMesh->HasBones())
		{
			std::cout << "Error: Mesh does not contain any bones." << std::endl;
			return nullptr;
		}
		else
		{
			std::cout << "Scene has " << pMesh->mNumBones << " bones." << std::endl;
		}


		if (pScene->mRootNode->mMetaData)
		{
			aiMetadata* pMetaData = pScene->mRootNode->mMetaData;
			std::cout << "Metadata: " << std::endl;
			for (uint32 i = 0; i < pMetaData->mNumProperties; i++)
			{
				std::cout << pMetaData->mKeys->C_Str() << std::endl;
			}
		}
		else
		{
			std::cout << "No metadata" << std::endl;
		}
		
		const aiBone* paiBones = pMesh->mBones[0];

		std::vector<MeshEntry> entries;
		entries.resize(pScene->mNumMeshes);
		std::cout << "Number of Meshes in file: " << entries.size() << std::endl;

		//making space for the vertex attributes and indices.
		std::vector<aiMesh*> paiMeshes;

		std::cout << "Loading data..." << std::endl;
		//counting the number of indices and vertices in mesh
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
		}

		//Extracting all the needed data from the mesh/meshes
		std::vector<BaseVertex> verts;
		std::vector<uint32> indices;
		pBones = new VertexBoneData[nrOfVerts];
		for (uint32 i = 0; i < entries.size(); i++)
		{
			for (uint32 j = 0; j < paiMeshes.at(i)->mNumVertices; j++)
			{
				BaseVertex data = {};
				data.Position = glm::vec3(paiMeshes.at(i)->mVertices[j].x, paiMeshes.at(i)->mVertices[j].y, paiMeshes.at(i)->mVertices[j].z);
				data.Normal = (paiMeshes.at(i)->HasNormals()) ? glm::vec3(paiMeshes.at(i)->mNormals[j].x, paiMeshes.at(i)->mNormals[j].y, paiMeshes.at(i)->mNormals[j].z) : glm::vec3();
				data.TexCoord = (paiMeshes.at(i)->HasTextureCoords(0)) ? glm::vec2(paiMeshes.at(i)->mTextureCoords[0][j].x, paiMeshes.at(i)->mTextureCoords[0][j].y) : glm::vec2();

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
				if (b >= MAX_NUM_BONES)
				{
					std::cout << "Number of bones exceeds limit of " << MAX_NUM_BONES << " bones." << std::endl;
					break;
				}

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
				}
			}

			//////load bones and send them to VAO
		}

		AnimatedMesh* pResult = new AnimatedMesh(verts.data(), indices.data(), pBones, nrOfVerts, nrOfIndices, nrOfbones, globalTransform, boneMap, boneOffsets, pRootNode, pScene, importer, entries);
		DeleteArrSafe(pBones);

		return pResult;
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
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumVerts * sizeof(BaseVertex), m_pVertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices * sizeof(uint32), m_pIndices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BaseVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//Normal
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(BaseVertex), (void*)(3 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(1));
	//TexCoords
	GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BaseVertex), (void*)(6 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(2));

	//Bone data that will be sent to shader for calculations.
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_BonesBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumVerts * sizeof(VertexBoneData), m_pBones, GL_STATIC_DRAW));

	//BoneID
	GL_CALL(glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)0));
	GL_CALL(glEnableVertexAttribArray(3));
	//BoneWeight
	GL_CALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)16));
	GL_CALL(glEnableVertexAttribArray(4));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	DeleteArrSafe(m_pVertices);
	DeleteArrSafe(m_pIndices);
	DeleteArrSafe(m_pBones);
}