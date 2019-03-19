#include "..\..\Include\GUI\UIReplay.h"
#include "../../Include/ReplayHandler.h"
#include "../../Include/Game.h"
#include "../../Include/Orders/OrderSchedule.h"
#include "../../Include/Scenarios/ScenarioManager.h"
#include <sstream>

UIReplay::UIReplay(float x, float y, float width, float height, float percentage) : ProgressBar(x, y, width, height)
{
	SetPercentage(percentage);
}

UIReplay::~UIReplay()
{
}

void UIReplay::PrintName() const
{
	std::cout << "UIReplay" << std::endl;
}

void UIReplay::RenderProgress(GUIContext* context, float x, float y) noexcept
{
	ProgressBar::RenderProgress(context, x, y);

	std::stringstream stream;
	stream.precision(3);
	stream << ReplayHandler::GetElapsedTime() << " s";
	static float width = 6.0F;
	Window& pWindow = Game::GetGame()->GetWindow();
	context->RenderText(stream.str(), GetWidth() * GetPercentage() - 20, y + GetHeight() * 2.0F, pWindow.GetWidth(), pWindow.GetHeight(), 0.5F, GUIContext::COLOR_WHITE, CENTER_VERTICAL);
	context->RenderTexture(GetDefaultTexture(), GetWidth() * GetPercentage() - width / 2.0F, y - GetHeight() / 2.0F, width, GetHeight() * 2, GUIContext::COLOR_WHITE);
}

void UIReplay::OnAdded(GUIObject * parent)
{
	AddRealTimeRenderer();
	AddMouseListener(this);
}

void UIReplay::OnRemoved(GUIObject * parent)
{
	RemoveRealTimeRenderer();
	RemoveMouseListener(this);
}

void UIReplay::OnUpdate(float dtS)
{
	if (Game::GetGame()->IsRenderingDisabled())
	{
		float currentPercentage = ReplayHandler::GetElapsedTime() / ReplayHandler::GetTotalTime();
		if (currentPercentage >= GetPercentage())
		{
			Game::GetGame()->DisableRenderingAndForceUpdate(false);
		}
	}
	else
	{
		SetPercentage(ReplayHandler::GetElapsedTime() / ReplayHandler::GetTotalTime());
	}
}

void UIReplay::OnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	if (ContainsPoint(position))
	{
		float percentage = (position.x - GetXInWorld()) / GetWidth();
		Game::GetGame()->DisableRenderingAndForceUpdate(true);
		SetPercentage(percentage);
		float currentPercentage = ReplayHandler::GetElapsedTime() / ReplayHandler::GetTotalTime();

		if (percentage < currentPercentage)
		{
			ReplayHandler::SoftReset();

			OrderSchedule::Release();
			ScenarioManager::Reset();
			ResourceHandler::ResetGameObjectCounters();

			World* pWorld = WorldSerializer::Read("world.json");

			////Enable clipplane for wallmaterial
			ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);
			ResourceHandler::GetMaterial(MATERIAL::BULKHEADS_STANDARD)->SetCullMode(CULL_MODE_NONE);

			SceneReplay* pSceneReplay = new SceneReplay(pWorld, percentage);

			pWorld->Generate(*pSceneReplay);

			pWorld->CopyRoomShadows(*pSceneReplay, Game::GetGame()->m_pSceneGame->GetWorld());

			Game::GetGame()->StartGame(pSceneReplay);
		}
	}
}