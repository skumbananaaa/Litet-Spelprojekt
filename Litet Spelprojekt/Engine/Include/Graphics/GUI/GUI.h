#include <EnginePch.h>
#include <Graphics/Textures/Framebuffer.h>

class API GUI
{
public:
	GUI(float width, float height);



	void GetWidth() const noexcept;
	void GetHeight() const noexcept;
	void GetX() const noexcept;
	void GetY() const noexcept;


private:
	Framebuffer* m_pFramebuffer;
	glm::vec2 m_position;
};