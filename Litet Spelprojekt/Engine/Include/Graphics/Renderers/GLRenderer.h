#pragma once
#include <EnginePch.h>
#include <Defines.h>
#include <Graphics\Geometry\IndexedMesh.h>
#include "..\Shaders\ShaderProgram.h"


enum Capability : GLuint
{
	DEPTH_TEST = GL_DEPTH_TEST,
	CULL_FACE = GL_CULL_FACE,
	BLEND = GL_BLEND
};

typedef Capability Cap;


class API GLRenderer
{
public:
	GLRenderer();
	~GLRenderer();

	void Enable(Cap cap) const noexcept;
	void Disable(Cap cap) const noexcept;
	void SetProgram(const ShaderProgram& program) const noexcept;

	void DrawIndexedMesh(const IndexedMesh& mesh) noexcept;
};