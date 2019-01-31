#pragma once

#include <System\Application.h>
#include <Graphics\GameObject.h>
#include <Graphics\Buffers\UniformBuffer.h>
#include <Graphics\Camera.h>
#include <Graphics\Scene.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>
#include <Graphics/GUI/PanelScrollable.h>
//#include "../../Game/Include/IO/WorldSerializer.h"

#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>

class Editor : public Application
{
public:
	Editor() noexcept;
	~Editor();

	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;

private:
	ShaderProgram* m_pShaderProgramDefault;

	Scene* m_pScene;

	std::vector<UniformBuffer*> m_GameObjectUniforms;

	UniformBuffer* m_pPerFrameUniform;
	float m_PerFrameArray[16 + 3];

	Mesh* m_pGridMesh;
	UniformBuffer* m_pGridUniform;

	TextView* m_pTextViewFPS;
	TextView* m_pTextViewUPS;
	Button* m_pButton;
	Button* m_pButton2;
	Slider* m_pSlider;
	Panel* m_pPanel;

	PanelScrollable* m_pPanelScrollable;
};