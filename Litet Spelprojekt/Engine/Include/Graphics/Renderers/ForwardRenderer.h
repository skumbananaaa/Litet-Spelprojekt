#pragma once
#include <Graphics/Materials/AnimatedSkeleton.h>
#include "IRenderer.h"
#include "TimerQuery.h"

struct ForwardFrameTimes
{
	float ReflectionPass = 0.0f;
	float SkyboxPass = 0.0f;
	float ParticlePass = 0.0f;
	float LightPass = 0.0f;
	float DepthPrePass = 0.0f;
	float AnimationPass = 0.0f;
};

class API ForwardRenderer : public IRenderer
{
public:
	ForwardRenderer(ForwardRenderer&& other) = delete;
	ForwardRenderer(const ForwardRenderer& other) = delete;
	ForwardRenderer& operator=(ForwardRenderer&& other) = delete;
	ForwardRenderer& operator=(const ForwardRenderer& other) = delete;

	ForwardRenderer();
	~ForwardRenderer();

	void SetClipDistance(const glm::vec4& plane, uint32 index) override final;
	void DrawScene(const Scene& scene, const World* pWorld, float dtS) const override final;
	void SetWorldBuffer(const Scene& scene, const World* pWorld) const override final;

private:
	void Create() noexcept;
	void CreateBatches(const Scene& scene, const World* const pWorld) const noexcept;
	void UpdateLightBuffer(const Scene& scene) const noexcept;
	void UpdateCameraBuffer(const Camera& camera) const noexcept;
	void UpdateExtensionBuffer(const Scene& scene) const noexcept;
	void UpdateShadowBuffer(const World* const pWorld) const noexcept;
	void ReflectionPass(const Scene& scene, const World* const pWorld) const noexcept;
	void DepthPrePass(const Camera& camera, const Scene& scene, const World* pWorld) const noexcept;
	void MainPass(const Camera& camera, const Scene& scene, const World* const pWorld) const noexcept;
	void AnimationPass(float dtS, const Scene& scene, const World* const pWorld) const noexcept;
	void ParticlePass(const Camera& camera, const Scene& scene) const noexcept;
	void SkyBoxPass(const Camera& camera, const Scene& scene) const noexcept;

private:
	UniformBuffer* m_pLightBuffer;
	UniformBuffer* m_pCameraBuffer;
	UniformBuffer* m_pMaterialBuffer;
	UniformBuffer* m_pPlaneBuffer;
	mutable WorldBuffer m_LocalWorldBuff = {};
	UniformBuffer* m_pWorldBuffer;
	UniformBuffer* m_pExtensionBuffer;
	UniformBuffer* m_pBoneBuffer;
	UniformBuffer* m_pShadowBuffer;

	UniformBuffer* m_pSkyBoxPassPerFrame;
	UniformBuffer* m_pSkyBoxPassPerObject;

	Particle* m_pParticle;

	const ShaderProgram* m_pParticleProgram;
	const ShaderProgram* m_pMeshParticleProgram;
	const ShaderProgram* m_pDepthPrePassProgram;
	const ShaderProgram* m_pAnimatedDepthPrePassProgram;
	const ShaderProgram* m_pSkyBoxPassProgram;

	mutable TimerQuery* m_pCurrentQuery;
	mutable TimerQuery* m_pQueries[2];
	mutable uint64 m_FrameCounter;
	mutable ForwardFrameTimes m_FrameTimes;
	mutable std::vector<DrawableBatch> m_DrawableBatches;
};