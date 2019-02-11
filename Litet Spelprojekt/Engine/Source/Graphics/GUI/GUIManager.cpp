#include <EnginePch.h>
#include <Graphics/GUI/GUIManager.h>

GUIManager::GUIManager(GLContext* glContext) : GUIObject(0, 0, 0, 0)
{
	FontRenderer* fontRenderer = FontRenderer::CreateFontRenderer("Resources/Fonts/arial.ttf", 10, 10);

	context = new GUIContext(glContext, ResourceHandler::GetShader(SHADER::GUI), fontRenderer);

	CreateDefaultTexture();
}

GUIManager::~GUIManager()
{
	delete context->GetFontRenderer();
	delete context;
	DeleteDefaultTexture();
}

float GUIManager::GetWidth() const noexcept
{
	return Window::GetCurrentWindow().GetWidth();
}

float GUIManager::GetHeight() const noexcept
{
	return Window::GetCurrentWindow().GetHeight();
}

void GUIManager::InternalRootOnRender()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	GUIObject::InternalRootOnRender(context);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void GUIManager::InternalRootOnUpdate(float dtS)
{
	InternalOnUpdate(dtS);
}

void GUIManager::InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	GUIObject::InternalRootOnMousePressed(position, mousebutton);
}

void GUIManager::InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	GUIObject::InternalRootOnMouseReleased(position, mousebutton);
}

void GUIManager::InternalRootOnMouseMove(const glm::vec2& position)
{
	GUIObject::InternalRootOnMouseMove(position);
}

void GUIManager::InternalRootOnMouseScroll(const glm::vec2& position, const glm::vec2& offset)
{
	GUIObject::InternalRootOnMouseScroll(position, offset);
}

void GUIManager::InternalRootOnKeyUp(KEY keycode)
{

}

void GUIManager::InternalRootOnKeyDown(KEY keycode)
{
	
}

void GUIManager::PrintName() const
{
	std::cout << "GUI Manager";
}
