#pragma once
#include <Graphics/Scene.h>
#include <System/Input.h>
#include <Graphics/Renderers/IRenderer.h>

class SceneInternal : public Scene
{
public:
	virtual ~SceneInternal();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept;
	virtual void OnDeactivated(SceneInternal* newScene) noexcept;

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
	SceneInternal(bool autoRotateCamera = true);

	IRenderer* GetRenderer() noexcept;

private:
	IRenderer* m_pRenderer;
	bool m_AutoRotateCamera;

	static float s_WaveX;
	static float s_WaveY;
};
