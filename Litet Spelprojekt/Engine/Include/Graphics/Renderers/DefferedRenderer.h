#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>
#include <IO/ResourceHandler.h>

#define LEVEL_SIZE_X 12
#define LEVEL_SIZE_Y 6
#define LEVEL_SIZE_Z 42

struct GPassVSPerFrame
{
	glm::mat4 ViewProjection;
	glm::vec3 CameraPosition;
	float Padding;
	glm::vec3 CameraLookAt;
	float Padding2;
	glm::vec4 ClipDistances[NUM_CLIP_DISTANCES];
};
//
//struct GeometryPassPerObject
//{
//	glm::vec4 Color;
//	
//	float HasTexture;
//	float HasNormalMap;
//	float DissolvePercentage;
//};

struct DecalPassPerFrame
{
	glm::mat4 ViewProj;
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
};

struct DecalPassPerObject
{
	float HasTexture;
	float HasNormalMap;
};

//struct WaterPassPerFrame
//{
//	glm::mat4 CameraCombined;
//	glm::vec3 CameraPosition;
//	float DistortionMoveFactor;
//};

//struct WaterPassPerObjectVS
//{
//	glm::mat4 Model;
//};

struct DrawableBatch
{
	const Material* pMaterial = nullptr;
	const IndexedMesh* pMesh = nullptr;
	std::vector<InstanceData> Instances;
};

struct DecalBatch
{
	const Decal* pDecal = nullptr;
	std::vector<InstanceData> Instances;
};

struct SkyBoxPassBuffer
{
	glm::mat4 CameraCombined;
	glm::vec4 CameraPosition;
};

struct SkyBoxPassPerObject
{
	glm::mat4 model;
};

struct PlaneBuffer
{
	glm::vec4 ClipPlane;
};

struct WorldBuffer
{
	uint32 map[LEVEL_SIZE_X][LEVEL_SIZE_Y][LEVEL_SIZE_Z];
	bool concealed;
	uint32 roomId;
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
	void DrawScene(const Scene& scene, float dtS) const override final;
	void SetWorldBuffer(const Scene& scene) const override final;

private:
	void Create() noexcept;
	void UpdateLightBuffer(const Scene& scene) const noexcept;
	void UpdateCameraBuffer(const Camera& camera) const noexcept;
	void UpdateWorldBuffer(const Scene& scene) const noexcept;
	void DecalPass(const Camera& camera, const Scene& scene) const noexcept;
	void GeometryPass(const Camera& camera, const Scene& scene) const noexcept;
	void GBufferResolvePass(const Camera& camera, const Scene& scene, const Framebuffer* const pGBuffer) const noexcept;
	void ReconstructionPass() const noexcept;
	void ForwardPass(const Camera& camera, const Scene& scene) const noexcept;
	void ReflectionPass(const Scene& sceen) const noexcept;
	void WaterPass(const Scene& sceen, float dtS) const noexcept;
	void SkyBoxPass(const Camera& camera, const Scene& screen) const noexcept;
	
	//DELETE?
	void DepthPrePass(const Scene& scene) const noexcept;

private:
	Framebuffer* m_pGBufferCBR;
	Framebuffer* m_pResolveTargets[2];
	Framebuffer* m_pForwardCBR;
	Framebuffer* m_pBlur;
	Framebuffer* m_pReflection;
	mutable Framebuffer* m_pCurrentResolveTarget;
	mutable Framebuffer* m_pLastResolveTarget;
	Texture2D* m_pForwardCBRTexture;

	FullscreenTri* m_pTriangle;
	
	UniformBuffer* m_pGeoPassPerFrame;
	UniformBuffer* m_pGeoPassPerObject;
	UniformBuffer* m_pLightPassBuffer;
	
	UniformBuffer* m_pLightBuffer;
	UniformBuffer* m_pCameraBuffer;
	UniformBuffer* m_pMaterialBuffer;
	UniformBuffer* m_pPlaneBuffer;

	mutable WorldBuffer m_LocalWorldBuff = {};
	UniformBuffer* m_pWorldBuffer;

	UniformBuffer* m_pDecalPassPerFrame;
	UniformBuffer* m_pDecalPassPerObject;

	UniformBuffer* m_pWaterPassPerFrame;
	UniformBuffer* m_pWaterPassPerObject;
	
	UniformBuffer* m_pSkyBoxPassPerFrame;
	UniformBuffer* m_pSkyBoxPassPerObject;
	
	IndexedMesh* m_pDecalMesh;
	
	Texture2D* m_pWaterNormalMap;
	Texture2D* m_pWaterDistortionMap;
	
	const ShaderProgram* m_pCbrBlurProgram;
	const ShaderProgram* m_pCbrReconstructionProgram;
	const ShaderProgram* m_pCbrResolveProgram;
	ShaderProgram* m_pGeometryPassProgram;
	const ShaderProgram* m_pDecalsPassProgram;
	const ShaderProgram* m_pForwardPass;
	const ShaderProgram* m_pWaterpassProgram;
	const ShaderProgram* m_pDepthPrePassProgram;
	const ShaderProgram* m_pSkyBoxPassProgram;

	glm::vec4 m_ClipDistances[NUM_CLIP_DISTANCES];
	
	mutable uint64 m_FrameCount;
	mutable std::vector<DrawableBatch> m_DrawableBatches;
	mutable std::vector<DecalBatch> m_DecalBatches;
};