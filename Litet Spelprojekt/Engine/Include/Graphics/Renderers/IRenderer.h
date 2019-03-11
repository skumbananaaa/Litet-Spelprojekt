#pragma once
#include <Graphics/Scene.h>
#include <World/World.h>

struct DrawableBatch
{
	const Material* pMaterial = nullptr;
	const IndexedMesh* pMesh = nullptr;
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

class API IRenderer
{
public:
	IRenderer(IRenderer&& other) = delete;
	IRenderer(const IRenderer& other) = delete;
	IRenderer& operator=(IRenderer&& other) = delete;
	IRenderer& operator=(const IRenderer& other) = delete;


	IRenderer() {};
	virtual ~IRenderer() {};

	virtual void DrawScene(const Scene& scene, const World* pWorld, float dtS) const = 0;
	virtual void SetParticleClipPlane(const glm::vec4& clipPlane) noexcept = 0;
};