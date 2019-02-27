#pragma once
#include "TextureCube.h"
#include "Framebuffer.h"

class API StaticShadowCube
{
public:
	StaticShadowCube(StaticShadowCube&& other) = delete;
	StaticShadowCube(const StaticShadowCube& other) = delete;
	StaticShadowCube& operator=(StaticShadowCube&& other) = delete;
	StaticShadowCube& operator=(const StaticShadowCube& other) = delete;

	StaticShadowCube();
	~StaticShadowCube();

private:
	void Create();

private:
	TextureCube* m_pCube;
};