#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/AnimatedMesh.h>

struct SkeletonBuffer
{
	glm::mat4 BoneTransforms[MAX_NUM_BONES];
	glm::mat4 Model;
};

class API AnimatedSkeleton
{
public:
	AnimatedSkeleton() noexcept;
	~AnimatedSkeleton();

	void SetSkeletonTransform(const glm::mat4& transform) noexcept;
	const SkeletonBuffer& GetSkeletonBuffer() const noexcept;

	void UpdateBoneTransforms(float dtS, const AnimatedMesh* mesh) const;

private:
	void ReadNodeHeirarchy(const void* pNode,
		const void* pScene,
		const glm::mat4& parentTransform,
		const glm::mat4& globalInverseTransform,
		std::map<std::string, uint32>& boneMap,
		std::vector<glm::mat4>& boneOffsets) const;

	const void* FindNodeAnim(const void* pAnimation, const std::string& NodeName) const;

private:
	mutable SkeletonBuffer m_SkeletonBuffer;
	mutable float m_AnimationTime;

};