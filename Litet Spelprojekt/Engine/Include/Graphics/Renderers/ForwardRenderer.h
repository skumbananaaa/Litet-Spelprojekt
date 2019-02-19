#pragma once
#include "IRenderer.h"
#include "TimerQuery.h"

#define LEVEL_SIZE_X 12
#define LEVEL_SIZE_Y 6
#define LEVEL_SIZE_Z 42

struct ForwardFrameTimes
{
	float ReflectionPass = 0.0f;
	float SkyboxPass = 0.0f;
	float ParticlePass = 0.0f;
	float LightPass = 0.0f;
	float DepthPrePass = 0.0f;
};

struct WorldBuffer
{
	int concealed;
	uint32 map[LEVEL_SIZE_X * LEVEL_SIZE_Y * LEVEL_SIZE_Z];
	int extended;
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
	void ReflectionPass(const Scene& scene) const noexcept;
	void DepthPrePass(const Camera& camera, const Scene& scene) const noexcept;
	void MainPass(const Camera& camera, const Scene& scene) const noexcept;
	void ParticlePass(const Camera& camera, const Scene& scene) const noexcept;
	void SkyBoxPass(const Camera& camera, const Scene& scene) const noexcept;

private:
	UniformBuffer* m_pLightBuffer;
	UniformBuffer* m_pCameraBuffer;
	UniformBuffer* m_pMaterialBuffer;
	UniformBuffer* m_pPlaneBuffer;

	UniformBuffer* m_pSkyBoxPassPerFrame;
	UniformBuffer* m_pSkyBoxPassPerObject;

	Particle* m_pParticle;

	const ShaderProgram* m_pParticleProgram;
	const ShaderProgram* m_pDepthPrePassProgram;
	const ShaderProgram* m_pSkyBoxPassProgram;

	mutable TimerQuery* m_pCurrentQuery;
	mutable TimerQuery* m_pQueries[2];
	mutable uint64 m_FrameCounter;
	mutable ForwardFrameTimes m_FrameTimes;
	mutable std::vector<DrawableBatch> m_DrawableBatches;
};