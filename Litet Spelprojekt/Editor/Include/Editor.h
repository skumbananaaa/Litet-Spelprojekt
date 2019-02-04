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

	static void OnButtonReleased(Button* button);
	static Editor* GetEditor();

private:
	ShaderProgram* m_pShaderProgramDefault;

	Scene* m_pScene;

	std::vector<UniformBuffer*> m_GameObjectUniforms;

	UniformBuffer* m_pPerFrameUniform;
	float m_PerFrameArray[16 + 3];

	Mesh* m_pGridMesh;
	UniformBuffer* m_pGridUniform;

	Button* m_pButtonSave;
	Button* m_pButtonLoad;
	Button* m_pButtonRoom;
	Button* m_pButtonMesh;
	Panel* m_pPanelTop;

	TextView* m_pTextViewFloor;
	Button* m_pButtonFloor1;
	Button* m_pButtonFloor2;
	Button* m_pButtonFloor3;
	Panel* m_pPanelFloor;

	TextView* m_pTextViewEditor;
	Button* m_pButtonAdd;
	Button* m_pButtonEdit;
	Button* m_pButtonRemove;
	Panel* m_pPanelEditor;
};