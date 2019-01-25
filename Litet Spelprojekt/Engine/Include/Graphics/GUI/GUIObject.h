#include <EnginePch.h>
#include <Graphics/Textures/Framebuffer.h>
#include <System/Input.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/FontRenderer.h>

class GUIManager;

class API GUIObject
{
	friend class GUIManager;

public:
	GUIObject(float x, float y, float width, float height);
	virtual ~GUIObject();

	float GetWidth() const noexcept;
	float GetHeight() const noexcept;
	float GetX() const noexcept;
	float GetY() const noexcept;

	int32 GetDepth() const noexcept;
	void SetDepth(int32 depth) noexcept;

	bool IsDirty() const noexcept;

protected:
	virtual void OnAdded(GUIManager* guiManager);
	virtual void OnRemoved(GUIManager* guiManager);

	virtual void OnUpdate(float dtS);
	virtual void OnRender(GLContext* context, FontRenderer* fontRenderer);

	virtual void OnMousePressed(MouseButton mousebutton);
	virtual void OnMouseReleased(MouseButton mousebutton);
	virtual void OnMouseMove(const glm::vec2& position);

	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);

	void RequestRepaint();

	Texture2D* m_pBackgroundTexture;

private:

	GUIManager* m_GUIManager;
	Framebuffer* m_pFramebuffer;
	glm::vec2 m_position;
	int32 m_Depth;
	bool m_IsDirty;
};