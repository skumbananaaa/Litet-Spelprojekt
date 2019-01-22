#pragma once
#include <EnginePch.h>
#include <Defines.h>
#include <Graphics/Geometry/Mesh.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Textures/Texture.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Textures/Framebuffer.h>

enum Capability : uint32
{
	DEPTH_TEST = 0,
	CULL_FACE = 1,
	BLEND = 2
};

enum PrimitiveTopology : unsigned int
{
	PT_POINTS = 0x0000,
	PT_LINES = 0x0001,
	PT_LINE_LOOP = 0x0002,
	PT_LINE_STRIP = 0x0003,
	PT_TRIANGLES = 0x0004,
	PT_TRIANGLE_STRIP = 0x0005,
	PT_TRIANGLE_FAN = 0x0006,
	PT_LINES_ADJACENCY = 0x000A,
	PT_LINE_STRIP_ADJACENCY = 0x000B,
	PT_TRIANGLES_ADJACENCY = 0x000C,
	PT_TRIANGLE_STRIP_ADJACENCY = 0x000D,
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
	void SetProgram(const ShaderProgram* pProgram) const noexcept;
	void SetTexture(const Texture* pTexture, uint32 slot) const noexcept;
	void SetUniformBuffer(const UniformBuffer* pBuffer, uint32 slot) const noexcept;
	void SetFramebuffer(const Framebuffer* pFramebuffer) const noexcept;
	void SetViewport(uint32 width, uint32 height, uint32 topX, uint32 topY) const noexcept;
	void SetClearColor(float r, float g, float b, float a) const noexcept;
	void Clear() const noexcept;
	void DrawIndexedMesh(const IndexedMesh& mesh) const noexcept;
	void DrawMesh(const Mesh& mesh, PrimitiveTopology primitiveTopology) const noexcept;
};