#include "..\Include\Game.h"
#include <World/Grid.h>
#include <System/Random.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <World/Grid.h>
#include <World/LightManager.h>
#include <Graphics/Particles/ParticleEmitter.h>

#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>

#include "../Include/Scenes/SceneLoading.h"

#if defined(_DEBUG)
//#define DRAW_DEBUG_BOXES
#endif

float g_Rot = 1.0;

Game::Game() noexcept
	: Application(false, 1600, 900, "", true),
	m_pSceneLast(nullptr),
	m_pScene(nullptr)
{
	SetScene(new SceneLoading());
}

Game::~Game()
{
	DeleteSafe(m_pScene);
}

void Game::OnResourceLoading(const std::string& file, float percentage)
{
	m_pScene->OnResourceLoading(file, percentage);
}

void Game::OnResourcesLoaded()
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::Init();
#endif

	m_pScene->OnResourcesLoaded();

	//m_pUIScenario = new UIScenario((GetWindow().GetWidth() - 600) / 2, (GetWindow().GetHeight() - 450) / 2, 600, 450);
}

void Game::OnKeyUp(KEY keycode)
{
	m_pScene->OnKeyUp(keycode);
}

void Game::OnKeyDown(KEY keycode)
{
	m_pScene->OnKeyDown(keycode);
}

void Game::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	m_pScene->OnMouseMove(lastPosition, position);
}

void Game::OnMouseReleased(MouseButton mousebutton, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->OwnsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI)
	{
		m_pScene->OnMouseReleased(mousebutton, position);
	}
}

void Game::OnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->OwnsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI)
	{
		m_pScene->OnMouseScroll(offset, position);
	}
}

void Game::OnUpdate(float dtS)
{
	if (m_pSceneLast)
	{
		DeleteSafe(m_pSceneLast);
	}

	static float waveX = 0.0f;
	static float waveY = 0.0f;
	waveX += 0.25f * dtS;
	waveY += 0.5f * dtS;
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetWaveFactor(glm::vec2(waveX, waveY));


	std::vector<PointLight*>& roomLights = m_Scenes[m_SceneId]->GetRoomLights();
	const std::vector<GameObject*>& drawables = m_Scenes[m_SceneId]->GetAnimatedDrawables();
	for (uint32 i = 0; i < roomLights.size(); i++)
	{
	
		m_RoomLightsTimers[i] += dtS;
		if (m_RoomLightsTimers[i] >= 5.0f )
		{
			roomLights[i]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			m_RoomLightsTimers[i] = 0.0f;
			m_pWorld->GetRoom(m_ActiveRooms[i])->SetActive(false);
			m_ActiveRooms[i] = 1;
			ScenarioManager::OnVisibilityChange(m_pWorld, m_Scenes[m_SceneId], m_ActiveRooms);
		}
	}

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_1);
#endif
	m_pScene->OnUpdate(dtS);

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Scene Update took %.3f ms", CPU_PROFILER_SLOT_1);
#endif

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::Update(dtS);
	CPUProfiler::PrintTime();
#endif
}

void Game::OnRender(float dtS)
{
	m_pScene->OnRender(dtS);
}

UIScenario* Game::GetUIScenario() noexcept
{
	return m_pUIScenario;
}

void Game::SetScene(SceneInternal* scene) noexcept
{
	m_pSceneLast = m_pScene;
	m_pScene = scene;
}

Game* Game::GetGame()
{
	return (Game*)(&Application::GetInstance());
}
