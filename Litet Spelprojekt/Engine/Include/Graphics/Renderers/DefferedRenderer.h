#pragma once
#include "IRenderer.h"

class API DefferedRenderer final : public IRenderer
{
public:
	DefferedRenderer();
	DefferedRenderer();

	void DrawScene(const Scene& scene) const override final;
};