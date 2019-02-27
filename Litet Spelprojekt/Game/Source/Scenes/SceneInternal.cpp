#include "../../Include/Scenes/SceneInternal.h"
#include <Graphics/Materials/WaterOutdoorMaterial.h>
#include <Graphics/GameObject.h>
#include "../../Include/Game.h"

float SceneInternal::s_WaveX = 0.0F;
float SceneInternal::s_WaveY = 0.0F;

SceneInternal::SceneInternal(bool autoRotateCamera) :
	m_AutoRotateCamera(autoRotateCamera)
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	Camera* pCamera = new Camera(glm::vec3(10.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.1f, 1000.0f);
	pCamera->UpdateFromLookAt();
	SetCamera(pCamera);

	GameObject* pGameObject = new GameObject();
	pGameObject->SetMesh(MESH::WATER_QUAD);
	pGameObject->SetMaterial(MATERIAL::WATER_OUTDOOR);
	AddGameObject(pGameObject);

	//Reflector for water
	PlanarReflector* pReflector = new PlanarReflector(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
	AddPlanarReflector(pReflector);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetStencilTest(true, FUNC_NOT_EQUAL, 0x00, 1, 0xff);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetStencilOp(STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_KEEP);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetPlanarReflector(pReflector);

	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	AddDirectionalLight(pDirectionalLight);
}

IRenderer* SceneInternal::GetRenderer() noexcept
{
	return m_pRenderer;
}

SceneInternal::~SceneInternal() 
{
	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();
}

void SceneInternal::OnActivated(SceneInternal* lastScene, IRenderer* renderer) noexcept
{
	m_pRenderer = renderer;
	if (lastScene)
	{
		GetCamera().SetPos(lastScene->GetCamera().GetPosition());
		GetCamera().SetYaw(lastScene->GetCamera().GetYaw());
		GetCamera().SetPitch(lastScene->GetCamera().GetPitch());
	}
}

void SceneInternal::OnDeactivated(SceneInternal* newScene) noexcept
{
}

void SceneInternal::OnUpdate(float dtS) noexcept
{
	Scene::OnUpdate(dtS);

	if (m_AutoRotateCamera)
	{
		GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, dtS * 0.1F);
		GetCamera().UpdateFromLookAt();
	}

	s_WaveX += 0.25f * dtS;
	s_WaveY += 0.5f * dtS;
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetWaveFactor(glm::vec2(s_WaveX, s_WaveY));
}

void SceneInternal::OnRender(float dtS) noexcept
{
	m_pRenderer->DrawScene(*this, nullptr, dtS);
}

void SceneInternal::OnResourceLoading(const std::string & file, float percentage)
{
}

void SceneInternal::OnResourcesLoaded()
{
}

void SceneInternal::OnMouseMove(const glm::vec2 & lastPosition, const glm::vec2 & position)
{
}

void SceneInternal::OnMouseScroll(const glm::vec2 & offset, const glm::vec2 & position)
{
}

void SceneInternal::OnMousePressed(MouseButton mousebutton, const glm::vec2 & position)
{
}

void SceneInternal::OnMouseReleased(MouseButton mousebutton, const glm::vec2 & position)
{
}

void SceneInternal::OnKeyUp(KEY keycode)
{
}

void SceneInternal::OnKeyDown(KEY keycode)
{
}

void SceneInternal::OnResize(uint32 width, uint32 height)
{
}