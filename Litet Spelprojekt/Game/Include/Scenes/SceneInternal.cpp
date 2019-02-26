#include "SceneInternal.h"
#include <Graphics/Materials/WaterOutdoorMaterial.h>
#include <Graphics/Renderers/ForwardRenderer.h>
#include <Graphics/GameObject.h>
#include "../../Include/Game.h"

SceneInternal::SceneInternal() :
	m_pRenderer(nullptr),
	m_pSkyBoxTex(nullptr)
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pRenderer = new ForwardRenderer();

	Camera* pCamera = new Camera(glm::vec3(-2.0f, 10.0f, 20.0f), glm::vec3(9.0f, 4.0f, 20.0f));
	float aspect = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	SetCamera(pCamera);

	GameObject* pGameObject = new GameObject();
	pGameObject->SetIsReflectable(true);
	pGameObject->SetMesh(MESH::QUAD);
	pGameObject->SetMaterial(MATERIAL::WATER_OUTDOOR);
	pGameObject->SetScale(glm::vec3(200.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	AddGameObject(pGameObject);

	//Reflector for water
	PlanarReflector* pReflector = new PlanarReflector(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
	AddPlanarReflector(pReflector);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetStencilTest(true, FUNC_NOT_EQUAL, 0x00, 1, 0xff);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetStencilOp(STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_KEEP);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetPlanarReflector(pReflector);

	//Skybox
	m_pSkyBoxTex = new TextureCube(ResourceHandler::GetTexture2D(TEXTURE::HDR));
	SetSkyBox(new SkyBox(m_pSkyBoxTex));
}

SceneInternal::~SceneInternal() 
{
	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pSkyBoxTex);
}

void SceneInternal::OnUpdate(float dtS) noexcept
{
	Scene::OnUpdate(dtS);

	static float dist = 0.0f;
	dist += 0.02f * dtS;
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetDistortionFactor(dist);
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