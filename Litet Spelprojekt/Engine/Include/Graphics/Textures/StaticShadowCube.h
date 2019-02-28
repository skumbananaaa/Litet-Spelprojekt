#pragma once
#include "TextureCube.h"
#include "Framebuffer.h"

class PointLight;
class Scene;

class API StaticShadowCube
{
public:
	StaticShadowCube(StaticShadowCube&& other) = delete;
	StaticShadowCube(const StaticShadowCube& other) = delete;
	StaticShadowCube& operator=(StaticShadowCube&& other) = delete;
	StaticShadowCube& operator=(const StaticShadowCube& other) = delete;

	StaticShadowCube(const PointLight& light, const Scene& scene);
	~StaticShadowCube();

private:
	void Create(const PointLight& light, const Scene& scene);

private:
	TextureCube* m_pCube;
};