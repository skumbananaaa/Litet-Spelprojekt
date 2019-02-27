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
	: Application(false, 1920, 1017, "", true),
	m_pSceneLast(nullptr),
	m_pScene(nullptr)
{
	SetScene(new SceneLoading());

	ScenarioManager::RegisterScenario(new ScenarioFire());
	ScenarioManager::RegisterScenario(new ScenarioWater(false));
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