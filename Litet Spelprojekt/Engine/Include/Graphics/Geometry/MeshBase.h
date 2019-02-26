#pragma once
#include <EnginePch.h>

//Base vertex
struct BaseVertex
{
public:
	BaseVertex(glm::vec3 position = glm::vec3(0.0f), glm::vec3 normal = glm::vec3(0.0f), glm::vec2 texCoord = glm::vec2(0.0f));
	
	bool operator==(const BaseVertex& other);
	bool operator!=(const BaseVertex& other);

public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

inline BaseVertex::BaseVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
{
	Position = position;
	Normal = normal;
	TexCoord = texCoord;
}

inline bool BaseVertex::operator==(const BaseVertex& other)
{
	return (Position == other.Position) && (Normal == other.Normal) && (TexCoord == other.TexCoord);
}

inline bool BaseVertex::operator!=(const BaseVertex& other)
{
	return !(*this == other);
}

//Instance
struct InstanceData
{
	glm::mat4 Model;
};

//Bonedata
struct VertexBoneData
{
public:
	glm::ivec4 IDs = glm::ivec4(0);
	glm::vec4 BoneWeights = glm::vec4(0.0f);

public:
	void AddBoneData(uint32 BoneID, float Weight);
};

inline void VertexBoneData::AddBoneData(uint32 BoneID, float Weight)
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