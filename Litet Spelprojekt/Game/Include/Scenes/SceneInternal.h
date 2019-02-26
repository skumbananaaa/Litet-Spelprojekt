#pragma once
#include <Graphics/Scene.h>
#include <System/Input.h>
#include <Graphics/Renderers/IRenderer.h>

class SceneInternal : public Scene
{
public:
	virtual ~SceneInternal();

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept;

	virtual void OnResourceLoading(const std::string& file, float percentage);
	virtual void OnResourcesLoaded();
	virtual void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position);
	virtual void OnMouseScroll(const glm::vec2& offset, const glm::vec2& position);
	virtual void OnMousePressed(MouseButton mousebutton, const glm::vec2& position);
	virtual void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position);
	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);
	virtual void OnResize(uint32 width, uint32 height);

protected:
	SceneInternal();

	IRenderer* m_pRenderer;

private:
	TextureCube* m_pSkyBoxTex;
};
