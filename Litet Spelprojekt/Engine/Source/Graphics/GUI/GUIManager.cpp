#include <EnginePch.h>
#include <Graphics/GUI/GUIManager.h>

GUIManager::GUIManager(GLContext* glContext) : GUIObject(0, 0, 0, 0), m_LastMousePosition(0, 0)
{
	Shader vShader;
	Shader fShader;

	vShader.CompileFromFile("Resources/Shaders/VShaderGUI.glsl", VERTEX_SHADER);
	fShader.CompileFromFile("Resources/Shaders/FShaderGUI.glsl", FRAGMENT_SHADER);

	FontRenderer* fontRenderer = FontRenderer::CreateFontRenderer("Resources/Fonts/arial.ttf", 10, 10);

	context = new GUIContext(glContext, new ShaderProgram(vShader, fShader), fontRenderer);

	CreateDefaultTexture();
}

GUIManager::~GUIManager()
{
	delete context->GetFontRenderer();
	delete context->GetShaderProgram();
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
}

void GUIManager::InternalRootOnUpdate(float dtS)
{
	InternalOnUpdate(dtS);
}

void GUIManager::InternalRootOnMousePressed(MouseButton mousebutton)
{
	GUIObject::InternalRootOnMousePressed(m_LastMousePosition, mousebutton);
}

void GUIManager::InternalRootOnMouseReleased(MouseButton mousebutton)
{
	GUIObject::InternalRootOnMouseReleased(m_LastMousePosition, mousebutton);
}

void GUIManager::InternalRootOnMouseMove(const glm::vec2& position)
{
	m_LastMousePosition = position;
	GUIObject::InternalRootOnMouseMove(position);
}

void GUIManager::InternalRootOnMouseScroll(const glm::vec2& offset)
{
	GUIObject::InternalRootOnMouseScroll(m_LastMousePosition, offset);
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
