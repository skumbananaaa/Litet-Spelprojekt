#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>
#include <IO/ResourceHandler.h>
#include "TimerQuery.h"

struct DecalBatch
{
	const Decal* pDecal = nullptr;
	std::vector<InstanceData> Instances;
};

struct DeferredFrameTimes
{
	float ReflectionPass = 0.0f;
	float SkyboxPass = 0.0f;
	float GeometryPass = 0.0f;
	float DecalPass = 0.0f;
	float ParticlePass = 0.0f;
	float LightPass = 0.0f;
	float ReconstructionPass = 0.0f;
};

class API DefferedRenderer final : public IRenderer
{
public:
	DefferedRenderer(DefferedRenderer&& other) = delete;
	DefferedRenderer(const DefferedRenderer& other) = delete;
	DefferedRenderer& operator=(DefferedRenderer&& other) = delete;
	DefferedRenderer& operator=(const DefferedRenderer& other) = delete;

	DefferedRenderer();
	~DefferedRenderer();

	void SetClipDistance(const glm::vec4& plane, uint32 index) override final;
	void DrawScene(const Scene& scene, const World* pWorld, float dtS) const override final;
	void SetWorldBuffer(const Scene& scene, const World* pWorld) const override final;

private:
	void Create() noexcept;
	void CreateBatches(const Scene& scene, const World* const pWorld) const noexcept;
	void UpdateLightBuffer(const Scene& scene) const noexcept;
	void UpdateCameraBuffer(const Camera& camera) const noexcept;
	void UpdateWorldBuffer(const Scene& scene) const noexcept;
	void GBufferResolvePass(const Camera& camera, const Scene& scene, const Framebuffer* const pGBuffer) const noexcept;
	void ReconstructionPass() const noexcept;
	void ReflectionPass(const Scene& scene) const noexcept;
	void GeometryPass(const Camera& camera, const Scene& scene) const noexcept;
	void DecalPass(const Camera& camera, const Scene& scene) const noexcept;
	void ParticlePass(const Camera& camera, const Scene& scene) const noexcept;
	void ForwardPass(const Camera& camera, const Scene& scene) const noexcept;
	void SkyBoxPass(const Camera& camera, const Scene& scene) const noexcept;
	
	//DELETE?
	void DepthPrePass(const Scene& scene) const noexcept;

private:
	Framebuffer* m_pGBufferCBR;
	Framebuffer* m_pResolveTarget;
	Framebuffer* m_pBlur;

	UniformBuffer* m_pLightBuffer;
	UniformBuffer* m_pCameraBuffer;
	UniformBuffer* m_pMaterialBuffer;
	UniformBuffer* m_pPlaneBuffer;
	mutable WorldBuffer m_LocalWorldBuff = {};
	UniformBuffer* m_pWorldBuffer;
	
	UniformBuffer* m_pSkyBoxPassPerFrame;
	UniformBuffer* m_pSkyBoxPassPerObject;
	
	Particle* m_pParticle;
	FullscreenTri* m_pTriangle;
	IndexedMesh* m_pDecalMesh;
	
	const ShaderProgram* m_pCbrBlurProgram;
	const ShaderProgram* m_pCbrReconstructionProgram;
	const ShaderProgram* m_pCbrResolveProgram;
	const ShaderProgram* m_pGeometryPassProgram;
	const ShaderProgram* m_pDecalsPassProgram;
	const ShaderProgram* m_pForwardPass;
	const ShaderProgram* m_pParticleProgram;
	const ShaderProgram* m_pDepthPrePassProgram;
	const ShaderProgram* m_pSkyBoxPassProgram;

	mutable TimerQuery* m_pCurrentQuery;
	mutable TimerQuery* m_pQueries[2];
	mutable uint64 m_FrameCounter;
	mutable DeferredFrameTimes m_FrameTimes;

	glm::vec4 m_ClipDistances[NUM_CLIP_DISTANCES];
	
	mutable std::vector<DrawableBatch> m_DrawableBatches;
	mutable std::vector<DecalBatch> m_DecalBatches;
};