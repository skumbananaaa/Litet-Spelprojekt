#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>
#include <IO/ResourceHandler.h>

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 18
#define NUM_SPOT_LIGHTS 8

struct GPassVSPerFrame
{
	glm::mat4 ViewProjection;
	glm::vec3 CameraPosition;
	float Padding;
	glm::vec3 CameraLookAt;
	float Padding2;
	glm::vec4 ClipDistances[NUM_CLIP_DISTANCES];
};

struct GeometryPassPerObject
{
	glm::vec4 Color;
	float HasTexture;
	float HasNormalMap;
};

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

//Uniformbuffers requires a 16 multiple so we pad 
//the struct in case we want to add more lights
__declspec(align(16)) struct DirectionalLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Direction = glm::vec3(0.0f);
};

__declspec(align(16)) struct PointLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Position = glm::vec3(0.0f);
};

__declspec(align(16)) struct SpotLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Position = glm::vec3(0.0f);
	float pad1;
	glm::vec3 Direction = glm::vec3(0.0f);
	float CutOffAngle = 1.0f;
	float OuterCutOffAngle = 1.0f;
};

struct LightPassBuffer
{
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
	glm::vec3 CameraPosition;
	float pad1;

};

struct LightBuffer
{
	DirectionalLightBuffer DirectionalLights[NUM_DIRECTIONAL_LIGHTS];
	PointLightBuffer PointLights[NUM_POINT_LIGHTS];
	SpotLightBuffer SpotLights[NUM_SPOT_LIGHTS];
};

struct WaterPassPerFrame
{
	glm::mat4 CameraCombined;
	glm::vec3 CameraPosition;
	float DistortionMoveFactor;
};

struct WaterPassPerObjectVS
{
	glm::mat4 Model;
};

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

private:
	void Create() noexcept;
	void UpdateLightBuffer(const Scene& scene) const noexcept;
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
	
	UniformBuffer* m_pDecalPassPerFrame;
	UniformBuffer* m_pDecalPassPerObject;

	UniformBuffer* m_pWaterPassPerFrame;
	UniformBuffer* m_pWaterPassPerObject;
	
	UniformBuffer* m_pSkyBoxPassPerFrame;
	UniformBuffer* m_pSkyBoxPassPerObject;
	
	IndexedMesh* m_pDecalMesh;
	
	Texture2D* m_pWaterNormalMap;
	Texture2D* m_pWaterDistortionMap;
	Texture2D* m_pDissolveMap;
	
	ShaderProgram* m_pCbrBlurProgram;
	ShaderProgram* m_pCbrReconstructionProgram;
	ShaderProgram* m_pCbrResolveProgram;
	ShaderProgram* m_pGeometryPassProgram;
	ShaderProgram* m_pDecalsPassProgram;
	ShaderProgram* m_pForwardPass;
	ShaderProgram* m_pWaterpassProgram;
	ShaderProgram* m_pDepthPrePassProgram;
	ShaderProgram* m_pSkyBoxPassProgram;

	glm::vec4 m_ClipDistances[NUM_CLIP_DISTANCES];
	
	mutable uint64 m_FrameCount;
	mutable std::vector<DrawableBatch> m_DrawableBatches;
	mutable std::vector<DecalBatch> m_DecalBatches;
};