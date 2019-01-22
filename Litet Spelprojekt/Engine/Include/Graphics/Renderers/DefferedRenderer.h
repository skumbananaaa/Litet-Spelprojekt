#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>

class API DefferedRenderer final : public IRenderer
{
public:
	DefferedRenderer();
	~DefferedRenderer();

	void DrawScene(const Scene& scene) const override final;

private:
	void Create();
	void GeometryPass(const Scene& scene);
	void LightPass(const Scene& scene);

private:
	Framebuffer* m_pGBuffer;
};