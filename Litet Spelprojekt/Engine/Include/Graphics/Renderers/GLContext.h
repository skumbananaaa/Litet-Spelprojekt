#pragma once
#include <EnginePch.h>
#include <Defines.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Textures/Texture.h>
#include <Graphics/Buffers/UniformBuffer.h>

enum Capability : GLuint
{
	DEPTH_TEST = GL_DEPTH_TEST,
	CULL_FACE = GL_CULL_FACE,
	BLEND = GL_BLEND
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
	void SetTexture(const Texture& texture, GLuint slot) const noexcept;
	void SetUniformBuffer(const UniformBuffer& buffer, GLuint slot) const noexcept;
	void DrawIndexedMesh(const IndexedMesh& mesh) const noexcept;
};