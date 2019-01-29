#include <EnginePch.h>
#include <Graphics/GUI/GUIManager.h>

GUIManager::GUIManager(GLContext* glContext) : GUIObject(0, 0, 0, 0, "GUI Manager")
{
	Shader vShader;
	Shader fShader;

	vShader.CompileFromFile("Resources/Shaders/VShaderGUI.glsl", VERTEX_SHADER);
	fShader.CompileFromFile("Resources/Shaders/FShaderGUI.glsl", FRAGMENT_SHADER);

	FontRenderer* fontRenderer = FontRenderer::CreateFontRenderer("Resources/Fonts/arial.ttf", 10, 10);

	context = new GUIContext(glContext, new ShaderProgram(vShader, fShader), fontRenderer);
}

GUIManager::~GUIManager()
{
	delete context->GetFontRenderer();
	delete context->GetShaderProgram();
	delete context;
}

void GUIManager::OnRender()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	InternalRootOnRender(context);

	glEnable(GL_DEPTH_TEST);
}

void GUIManager::InternalRootOnUpdate(float dtS)
{
	InternalOnUpdate(dtS);
}

void GUIManager::OnMousePressed(MouseButton mousebutton)
{
	
}

void GUIManager::OnMouseReleased(MouseButton mousebutton)
{
	
}

void GUIManager::OnMouseMove(const glm::vec2& position)
{
	
}

void GUIManager::OnKeyUp(KEY keycode)
{
	
}

void GUIManager::OnKeyDown(KEY keycode)
{
	
}