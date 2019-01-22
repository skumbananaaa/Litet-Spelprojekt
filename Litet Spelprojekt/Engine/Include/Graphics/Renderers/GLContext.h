#pragma once
#include <EnginePch.h>
#include <Defines.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Textures/Texture.h>
#include <Graphics/Buffers/UniformBuffer.h>

enum Capability : uint32
{
	DEPTH_TEST = 0,
	CULL_FACE = 1,
	BLEND = 2
};

typedef Capability Cap;


class API GLContext
{
public:
	GLContext(GLContext&& other) = delete;
	GLContext(const GLContext& other) = delete;
	GLContext& operator=(GLContext&& other) = delete;
	GLContext& operator=(const GLContext& other) = delete;

	GLContext();
	~GLContext();

	void Enable(Cap cap) const noexcept;
	void Disable(Cap cap) const noexcept;
	void SetProgram(const ShaderProgram& program) const noexcept;
	void SetTexture(const Texture& texture, uint32 slot) const noexcept;
	void SetUniformBuffer(const UniformBuffer& buffer, uint32 slot) const noexcept;
	void SetViewport(uint32 width, uint32 height, uint32 topX, uint32 topY) const noexcept;
	void SetClearColor(float r, float g, float b, float a) const noexcept;
	void Clear() const noexcept;
	void DrawIndexedMesh(const IndexedMesh& mesh) const noexcept;
};