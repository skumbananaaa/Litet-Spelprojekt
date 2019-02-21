#include <EnginePch.h>
#include <Graphics/Materials/AnimatedSkeleton.h>
#include <Graphics/Geometry/AnimatedMesh.h>

#include <Graphics/Geometry/AssimpHelper.inl>

void CalcInterpolatedScaling(aiVector3D& scaleVector, float animationTime, const aiNodeAnim* nodeAnim);
void CalcInterpolatedRotation(aiQuaternion& rotationVector, float animationTime, const aiNodeAnim* nodeAnim);
void CalcInterpolatedPosition(aiVector3D& positionVector, float animationTime, const aiNodeAnim* nodeAnim);
//Following functions finds the key position which is immediately before the animation time. 
//If we have N key position the result can be 0 to N-2. 
//The animation time is always contained inside the duration of the channel so the last key (N-1) can never be a valid result. 
uint32 FindPosition(const aiNodeAnim* pNodeAnim, float animationTime);
uint32 FindRotation(const aiNodeAnim* pNodeAnim, float animationTime);
uint32 FindScaling(const aiNodeAnim* pNodeAnim, float animationTime);

AnimatedSkeleton::AnimatedSkeleton() noexcept :
	m_AnimationTime(0.0f)
{
	for (uint32 i = 0; i < MAX_NUM_BONES; i++)
	{
		m_SkeletonBuffer.BoneTransforms[i] = glm::mat4(1.0f);
	}

	m_SkeletonBuffer.Model = glm::mat4(1.0f);
}

AnimatedSkeleton::~AnimatedSkeleton()
{
}

void AnimatedSkeleton::SetSkeletonTransform(const glm::mat4& transform) noexcept
{
	m_SkeletonBuffer.Model = transform;
}

const SkeletonBuffer& AnimatedSkeleton::GetSkeletonBuffer() const noexcept
{
	return m_SkeletonBuffer;
}

void AnimatedSkeleton::UpdateBoneTransforms(float dtS, const AnimatedMesh* mesh) const
{
	glm::mat4 identity(1.0);

	const aiScene* scene = reinterpret_cast<const Assimp::Importer*>(mesh->m_pImporter)->GetScene();
	float ticksPerSec = (float)scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = m_AnimationTime * ticksPerSec;
	m_AnimationTime = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(scene->mRootNode, scene, identity, mesh->m_GlobalInverseTransform, mesh->m_BoneMap, mesh->m_BoneOffsets);
	m_AnimationTime += dtS;
}

void CalcInterpolatedPosition(aiVector3D& positionVector, float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumPositionKeys == 1) 
	{
		positionVector = nodeAnim->mPositionKeys[0].mValue;
		return;
	}

	uint32 PositionIndex = FindPosition(nodeAnim, animationTime);
	uint32 NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < nodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (animationTime - (float)nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	positionVector = Start + Factor * Delta;
}

void CalcInterpolatedScaling(aiVector3D& scaleVector, float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumScalingKeys == 1) 
	{
		scaleVector = nodeAnim->mScalingKeys[0].mValue;
		return;
	}
	//scaling index for specific key? tror jag :|
	uint32 ScalingIndex = FindScaling(nodeAnim, animationTime);
	uint32 NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < nodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(nodeAnim->mScalingKeys[NextScalingIndex].mTime - nodeAnim->mScalingKeys[ScalingIndex].mTime);//time between key frames
	float Factor = (animationTime - (float)nodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = nodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = nodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	scaleVector = Start + Factor * Delta;
}

void CalcInterpolatedRotation(aiQuaternion& rotationVector, float animationTime, const aiNodeAnim* nodeAnim)
{
	// we need at least two values to interpolate...
	if (nodeAnim->mNumRotationKeys == 1) 
	{
		rotationVector = nodeAnim->mRotationKeys[0].mValue;
		return;
	}

	uint32 RotationIndex = FindRotation(nodeAnim, animationTime);
	uint32 NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < nodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(nodeAnim->mRotationKeys[NextRotationIndex].mTime - nodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (animationTime - (float)nodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(rotationVector, StartRotationQ, EndRotationQ, Factor);
	rotationVector = rotationVector.Normalize();
}

uint32 FindPosition(const aiNodeAnim* pNodeAnim, float animationTime)
{
	for (uint32 i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
	{
		if (animationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

uint32 FindRotation(const aiNodeAnim* pNodeAnim, float animationTime)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (uint32 i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) 
	{
		if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}


uint32 FindScaling(const aiNodeAnim* pNodeAnim, float animationTime)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (uint32 i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) 
	{
		if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

void AnimatedSkeleton::ReadNodeHeirarchy(const void* pNode, 
	const void* pScene,
	const glm::mat4& parentTransform, 
	const glm::mat4& globalInverseTransform, 
	std::map<std::string, uint32>& boneMap, 
	std::vector<glm::mat4>& boneOffsets) const
{
	const aiNode* pCurrentNode = reinterpret_cast<const aiNode*>(pNode);
	const aiScene* pCurrentScene = reinterpret_cast<const aiScene*>(pScene);

	std::string NodeName(pCurrentNode->mName.data);

	const aiAnimation* pAnimation = pCurrentScene->mAnimations[0];

	glm::mat4 nodeTransformation = AssimpToGLMMat4(pCurrentNode->mTransformation);

	const aiNodeAnim* pNodeAnim = reinterpret_cast<const aiNodeAnim*>(FindNodeAnim(pAnimation, NodeName));

	if (pNodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D scalingVec;
		CalcInterpolatedScaling(scalingVec, m_AnimationTime, pNodeAnim);
		glm::mat4 scalingMat4;
		scalingMat4 = AssimpScaleMat4(scalingVec);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, m_AnimationTime, pNodeAnim);
		aiMatrix4x4 tempMat4(RotationQ.GetMatrix());
		glm::mat4 rotationMat4 = AssimpToGLMMat4(tempMat4);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, m_AnimationTime, pNodeAnim);
		glm::mat4 translationMat4;
		translationMat4 = AssimpTranslateMat4(Translation);

		// Combine the above transformations
		nodeTransformation = translationMat4 * rotationMat4 * scalingMat4;
	}

	const glm::mat4 globalTransformation = parentTransform * nodeTransformation;

	if (boneMap.find(NodeName) != boneMap.end())
	{
		uint32 BoneIndex = boneMap[NodeName];
		m_SkeletonBuffer.BoneTransforms[BoneIndex] = globalInverseTransform * globalTransformation * boneOffsets[BoneIndex];

		//std::cout << BoneIndex << ", " << glm::to_string(boneInfo[BoneIndex].finalTransform) << std::endl;
	}

	for (uint32 i = 0; i < pCurrentNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(pCurrentNode->mChildren[i], pScene, globalTransformation, globalInverseTransform, boneMap, boneOffsets);
	}

	return;
}

const void* AnimatedSkeleton::FindNodeAnim(const void* pAnimation, const std::string& NodeName) const
{
	const aiAnimation* pCurrentAnimation = reinterpret_cast<const aiAnimation*>(pAnimation);

	for (uint32 i = 0; i < pCurrentAnimation->mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = pCurrentAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}
