#pragma once
#include <Graphics/Scene.h>

class API IRenderer
{
public:
	IRenderer(IRenderer&& other) = delete;
	IRenderer(const IRenderer& other) = delete;
	IRenderer& operator=(IRenderer&& other) = delete;
	IRenderer& operator=(const IRenderer& other) = delete;

	IRenderer() {};
	virtual ~IRenderer() {};

	virtual void SetClipDistance(const glm::vec4& plane, uint32 index) = 0;
	virtual void DrawScene(const Scene& scene, float dtS) const = 0;
	virtual void SetWorldBuffer(const Scene& scene) const = 0;
};