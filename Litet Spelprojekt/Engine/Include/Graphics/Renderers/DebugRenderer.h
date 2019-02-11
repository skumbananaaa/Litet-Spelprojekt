#pragma once
#include <Graphics/Scene.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <IO/ResourceHandler.h>

class UniformBuffer;
class IndexedMesh;

struct DebugPerFrame
{
	glm::mat4 ProjectionView;
};

struct DebugPerObject
{
	glm::mat4 Model;
};

class API DebugRenderer
{
public:
	DebugRenderer(DebugRenderer&& other) = delete;
	DebugRenderer(const DebugRenderer& other) = delete;
	DebugRenderer& operator=(DebugRenderer&& other) = delete;
	DebugRenderer& operator=(const DebugRenderer& other) = delete;

	DebugRenderer();
	~DebugRenderer();

	void DrawScene(const Scene& scene) const noexcept;

private:
	void Create() noexcept;
	void DrawDecals(const Scene& scene) const noexcept;

private:
	const ShaderProgram* m_pDecalProgram;
	
	UniformBuffer* m_pPerFrame;
	UniformBuffer* m_pPerObject;

	IndexedMesh* m_pUnitCubeMesh;
};
