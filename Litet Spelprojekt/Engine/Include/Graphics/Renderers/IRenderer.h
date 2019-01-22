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

	virtual void DrawScene(const Scene& scene) const = 0;
};