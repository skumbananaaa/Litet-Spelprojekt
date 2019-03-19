#include "../../Include/Game.h"
#include <World/LightManager.h>
#include <Graphics/Textures/StaticShadowCube.h>
#include "../../Include/Scenarios/ScenarioManager.h"
#include "../../Include/GameObjectDoor.h"
#include "../../Include/Orders/OrderSleep.h"
#include "../../Include/Orders/OrderSchedule.h"
#include "../../Include/Orders/OrderGiveAid.h"
#include "../../Include/GameState.h"
#include <Graphics/Materials/MaterialBase.h>
#include "../../Include/Orders/OrderPlugHole.h"
#include "../../Include/Scenarios/ScenarioWater.h"

SceneGame::SceneGame(World* pWorld) 
	: SceneInternal(false),
	m_pWorld(pWorld),
	m_pTestAudioSource(nullptr),
	m_pUIPause(nullptr),
	m_pUIEndScreen(nullptr),
	m_IsPaused(false),
	m_pUIRequest(nullptr),
	m_IsGameOver(false),
	m_pLookAt(nullptr)
{
	LightManager::Init(this, NUM_SPOT_LIGHTS);

	ScenarioManager::Init(m_pWorld);

	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetStencilTest(true, FUNC_ALWAYS, 0xff, 1, 0xff);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetFrontFaceStencilOp(STENCIL_OP_KEEP, STENCIL_OP_REPLACE, STENCIL_OP_ZERO);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetBackFaceStencilOp(STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_REPLACE);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);

	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);

	GetCamera().SetMaxPitch(-glm::two_pi<float>() / 64.0f);
	GetCamera().SetMinXZMaxXZLookAt(1.0f, 1.0f, 11.0f, 41.0f);
}

SceneGame::~SceneGame()
{
	ScenarioManager::Reset();

	DeleteSafe(m_pWorld);
	DeleteSafe(m_pUICrew);
	DeleteSafe(m_pUINotification);
	DeleteSafe(m_pTestAudioSource);

	Logger::Save();
	LightManager::Release();
}

void SceneGame::OnActivated(SceneInternal* lastScene, IRenderer* pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, pRenderer);

	CreateAudio();
	CreateGameObjects();
	CreateCrew();

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pUICrewMember = new UICrewMember((window->GetWidth() - 330) / 2.0f, window->GetHeight() - 170.0f, 330.0f, 170.0f);
	m_pUIRequest = new UICrewRequest(window->GetWidth() / 4.0f, window->GetHeight() - 50.0f, 200.0f, 50.0f);
	m_pUILog = new UILog(window->GetWidth() - 600.0f, window->GetHeight() - 700.0f, 600.0f, 700.0f);
	game->GetGUIManager().Add(m_pUICrewMember);
	game->GetGUIManager().Add(m_pUILog);
	game->GetGUIManager().Add(m_pUIRequest);

	Logger::SetListener(m_pUILog);

	m_pUICrew = new UICrew(0.0f, window->GetHeight() - 50.0f, 200.0f, 500.0f, &m_Crew);
	m_pUINotification = new UINotification(window->GetWidth() - 560.0f, (float)window->GetHeight(), 500.0f, 50.0f, 8.0F);

	SetPaused(false);

	OrderSchedule::Init(this);
	for (uint32 i = 0; i < m_Crew.GetCount(); i++)
	{
		IOrder* pOrder = OrderSchedule::GetIdleOrder();
		if (pOrder)
		{
			m_Crew.GetMember(i)->GiveOrder(pOrder);
		}
	}

	GameState::Reset();
	UpdateMaterialClipPlanes();
}

void SceneGame::OnDeactivated(SceneInternal* newScene) noexcept
{
	SceneInternal::OnDeactivated(newScene);

	GLContext& context = Application::GetInstance().GetGraphicsContext();
	const std::vector<PlanarReflector*>& reflectors = GetPlanarReflectors();
	for (size_t i = 0; i < reflectors.size(); i++)
	{
		const Framebuffer* pFramebuffer = reflectors[i]->GetFramebuffer();

		context.SetViewport(pFramebuffer->GetWidth(), pFramebuffer->GetHeight(), 0, 0);
		context.SetFramebuffer(pFramebuffer);
		context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);
	}

	GetCamera().SetMaxPitch(1.55334303f);

	OrderSchedule::Release();
	ResourceHandler::ResetGameObjectCounters();
}

void SceneGame::OnUpdate(float dtS) noexcept
{
	if (m_IsPaused && !IsPaused())
	{
		Game* game = Game::GetGame();
		Window* window = &game->GetWindow();
		m_pUIPause = new UIPause(0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight());
		SetUIVisible(false);
		game->GetGUIManager().Add(m_pUIPause);
	}
	else if (IsPaused() && !m_IsPaused)
	{
		Game* game = Game::GetGame();
		SetUIVisible(true);
		game->GetGUIManager().Remove(m_pUIPause);
		m_pUIPause = nullptr;
	}

	if (m_IsGameOver && !m_pUIEndScreen)
	{
		Game* game = Game::GetGame();
		Window* window = &game->GetWindow();
		game->GetGUIManager().DeleteChildren();
		m_pUIEndScreen = new UIEndScreen((window->GetWidth() - 800) / 2, (window->GetHeight() - 800) / 2, 800, 800, !GameState::HasCompletedScenarios());
		game->GetGUIManager().Add(m_pUIEndScreen);
	}

	if (!IsPaused() && !m_IsGameOver)
	{
		if (GameState::GetWaterLeakAmount() > MAX_WATERLEAKAGE|| GameState::GetBurningAmount() > MAX_SHIPDAMAGE|| GameState::GetCrewHealth() < MIN_CREWHEALTH || GameState::HasCompletedScenarios())
		{
			m_IsGameOver = true;
		}

		m_pUINotification->Update(dtS);

		SceneInternal::OnUpdate(dtS);
		ScenarioManager::Update(dtS, m_pWorld, this);
		UpdateCamera(dtS);

		for (uint32 i = 0; i < MAX_NUM_ROOMS; i++)
		{
			Room& room = m_pWorld->GetRoom(i);

			if (room.IsRoomInitialized())
			{
				m_pWorld->GetRoom(i).SetFloodUpdated(false);
			}
		}

		static float dist = 0.0f;
		dist += 0.02f * dtS;
		((WaterIndoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_INDOOR))->SetDistortionFactor(dist);

#if defined(PRINT_CPU_DEBUG_DATA)
		CPUProfiler::StartTimer(CPU_PROFILER_SLOT_0);
#endif
		if (m_pWorld->UpdateVisibility(*this, dtS))
		{
			ScenarioManager::OnVisibilityChange(m_pWorld, this);
		}

#if defined(PRINT_CPU_DEBUG_DATA)
		CPUProfiler::EndTimer("World Update took %.3f ms", CPU_PROFILER_SLOT_0);
#endif

		GameObject* pCameraLookAt = GetGameObject("cameraLookAt");
		if (pCameraLookAt)
		{
			pCameraLookAt->SetPosition(GetCamera().GetLookAt());
		}

		AudioListener::SetPosition(GetCamera().GetPosition());
		AudioListener::SetOrientation(GetCamera().GetFront(), GetCamera().GetUp());
	}
}

void SceneGame::OnRender(float dtS) noexcept
{
	GetRenderer()->DrawScene(*this, m_pWorld, dtS);

#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_Scenes[m_SceneId]);
#endif
}

void SceneGame::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (!IsPaused() && !m_IsGameOver)
	{
		if (Input::IsKeyDown(KEY_LEFT_ALT))
		{
			Camera& camera = GetCamera();

			if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
			{
				const float cameraRotationSensitivity = 0.005f;
				glm::vec2 deltaPosition = cameraRotationSensitivity * (position - lastPosition);

				camera.MoveRelativeLookAt(PosRelativeLookAt::RotateX, deltaPosition.x);
				camera.MoveRelativeLookAt(PosRelativeLookAt::RotateY, -deltaPosition.y);

				m_pUICrewMember->SetCrewMember(nullptr);
			}

			if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
			{
				const float cameraMoveSensitivityX = 0.5f;
				const float cameraMoveSensitivityY = 0.025f;
				glm::vec2 deltaPosition = cameraMoveSensitivityY * (position - lastPosition);
				glm::vec3 moveOffset(0.0f);
				moveOffset.x = camera.GetFront().x;
				moveOffset.z = camera.GetFront().z;
				moveOffset = glm::normalize(moveOffset) *  -deltaPosition.y;
				moveOffset += camera.GetMoveWorldFromLocal(glm::vec3(cameraMoveSensitivityX * deltaPosition.x, 0.0f, 0.0f));

				if (IsExtended())
				{
					glm::vec3 oldLookAt = camera.GetLookAt();
					glm::vec3 newLookAt = oldLookAt + moveOffset;

					if (!camera.IsLookAtInBounds(newLookAt) &&
						newLookAt.x >= 1.0f && newLookAt.x <= 31.0f &&
						newLookAt.z >= 1.0f && newLookAt.z <= 41.0f)
					{
						float xMove = -(float)IsExtended() * 10.0f;
						float yOffset = -2.0f * (glm::floor(((((uint32)newLookAt.x - 1) % 10) / 5.0f)) - 0.5f);
						float newY =  (oldLookAt.y / 2.0f) + yOffset;
						float lookAtBoundsOffset = -xMove * newY;

						camera.SetMinXZMaxXZLookAt(lookAtBoundsOffset + 1.0f, 1.0f,
							lookAtBoundsOffset + 11.0f, 41.0f);

						moveOffset.y = 2.0f * yOffset;
					}
				}

				camera.MoveWorldCoords(moveOffset, true);

				m_pUICrewMember->SetCrewMember(nullptr);
			}
		}
		else
		{
			Pick(true, (int32)position.x, (int32)position.y);
		}
	}
}

void SceneGame::OnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	if (!IsPaused() && !m_IsGameOver)
	{
		Camera& camera = GetCamera();

		if (Input::IsKeyDown(KEY_LEFT_ALT))
		{
			if (offset.y > 0.0f)
			{
				if (camera.GetLookAt().y < 4.0f)
				{
					float xMove = (float)IsExtended() * 10.0f;
					float lookAtBoundsOffset = xMove * (camera.GetLookAt().y / 2.0f + 1.0f);
					camera.SetMinXZMaxXZLookAt(lookAtBoundsOffset + 1.0f, 1.0f,
											   lookAtBoundsOffset + 11.0f, 41.0f);
					camera.MoveWorldCoords(glm::vec3(xMove, 2.0f, 0.0f), true);
					UpdateMaterialClipPlanes();
				}
			}
			else
			{
				if (camera.GetLookAt().y > 0.0f)
				{
					float xMove = -(float)IsExtended() * 10.0f;
					float lookAtBoundsOffset = -xMove * (camera.GetLookAt().y / 2.0f - 1.0f);
					camera.SetMinXZMaxXZLookAt(lookAtBoundsOffset + 1.0f, 1.0f,
											   lookAtBoundsOffset + 11.0f, 41.0f);
					camera.MoveWorldCoords(glm::vec3(xMove, -2.0f, 0.0f), true);
					UpdateMaterialClipPlanes();
				}
			}
		}
		else
		{
			const float cameraZoomSensitivity = 0.1f;
			const glm::vec2& cNearFar = camera.GetMinMaxDistToLookAt();
			float distanceBoost = glm::max(15.0f * camera.GetDistanceToLookAt() / cNearFar.y, 1.0f);
			camera.MoveRelativeLookAt(PosRelativeLookAt::Zoom, cameraZoomSensitivity * offset.y * distanceBoost);
		}
	}
}

void SceneGame::OnMousePressed(MouseButton mousebutton, const glm::vec2& position)
{
}

void SceneGame::OnMouseReleased(MouseButton mousebutton, const glm::vec2& position)
{
	if (!IsPaused() && !m_IsGameOver)
	{
		switch (mousebutton)
		{
			case MOUSE_BUTTON_RIGHT:
			{
				if (!Input::IsKeyDown(KEY_LEFT_ALT) && m_pWorld != nullptr)
				{
					PickPosition();
				}
				break;
			}
			case MOUSE_BUTTON_LEFT:
			{
				if (!Input::IsKeyDown(KEY_LEFT_ALT) && m_pWorld != nullptr)
				{
					Pick(false, (int32)position.x, (int32)position.y);
				}
				break;
			}
			case MOUSE_BUTTON_MIDDLE:
			{
				for (uint32 i = 0; i < m_Crew.GetCount(); i++)
				{
					if (m_Crew.GetMember(i)->IsPicked())
					{
						m_Crew.GetMember(i)->GoToSickBay();
					}
				}
				break;
			}
		}
	}
}

void SceneGame::OnKeyUp(KEY keycode)
{
}

void SceneGame::OnKeyDown(KEY keycode)
{
	if (!m_IsGameOver)
	{
		if (keycode == KEY_ESCAPE)
		{
			SetPaused(!IsPaused());
		}
		else if (!IsPaused())
		{
			switch (keycode)
			{
				case KEY_P:
				{
					m_pTestAudioSource->TogglePause();
					break;
				}
				case KEY_SPACE:
				{
					ExtendScene();
					UpdateMaterialClipPlanes();
					break;
				}
				case KEY_R:
				{
					ShowCrewmember(0);
					ScenarioManager::OnVisibilityChange(m_pWorld, this);
					break;
				}
				case KEY_G:
				{
					m_Crew.GetMember(0)->GiveOrder(OrderSchedule::GetIdleOrder());
					break;
				}
				case KEY_H:
				{
					Crewmember* medic = nullptr;
					Crewmember* victim = nullptr;
					for (uint32 i = 0; i < m_Crew.GetCount(); i++)
				{
					Crewmember* member = m_Crew.GetMember(i);
					if (!member->HasRecovered() && !member->IsAbleToWork() && (m_pWorld->GetRoom(member->GetRoom()).GetCenter() == m_pWorld->GetRoom(SICKBAY_0).GetCenter() || m_pWorld->GetRoom(member->GetRoom()).GetCenter() == m_pWorld->GetRoom(SICKBAY_1).GetCenter()))
					{
						victim = member;
					}
					else if (member->GetGroupType() == MEDIC)
					{
						medic = member;
					}

					if (medic != nullptr && victim != nullptr)
					{
						break;
					}
				}
					if (medic != nullptr && victim != nullptr)
				{
					medic->GiveOrder(new OrderGiveAid(victim));
				}
					break;
				}
				case KEY_ENTER:
				{
					const std::vector<int32>& selectedList = m_Crew.GetSelectedList();
					for (int32 i = 0; i < selectedList.size(); i++)
					{
						m_Crew.GetMember(selectedList[i])->RequestReportPosition();
					}
					break;
				}
			}
		}
	}
}

void SceneGame::OnResize(uint32 width, uint32 height)
{

}

void SceneGame::OnSceneExtensionComplete() noexcept
{
	UpdateMaterialClipPlanes();
}

void SceneGame::OnGameOver() noexcept
{
	std::cout << "GAME OVER" << std::endl;
}

void SceneGame::CreateAudio() noexcept
{
	AudioListener::SetPosition(glm::vec3(0.0f));
	m_pTestAudioSource = AudioSource::CreateMusicSource(MUSIC::WAVES_AND_SEAGULLS);
	m_pTestAudioSource->SetVolume(0.4);
	m_pTestAudioSource->SetPitch(1.0f);
	m_pTestAudioSource->SetLooping(true);
	m_pTestAudioSource->Play();
}

void SceneGame::CreateGameObjects() noexcept
{
	GameObject* pGameObject = nullptr;
	{
		//Look at
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::LOOK_AT);
			pGameObject->SetMesh(MESH::QUAD);
			pGameObject->SetPosition(GetCamera().GetLookAt() + glm::vec3(0.0f, 0.06f, 0.0f));
			pGameObject->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
			pGameObject->UpdateTransform();
			pGameObject->SetWorld(m_pWorld);
			AddGameObject(pGameObject);
			m_pLookAt = pGameObject;
			pGameObject = nullptr;
		}
	}
}

void SceneGame::CreateWorld() noexcept
{
	m_pWorld = WorldSerializer::Read("world.json");

	////Enable clipplane for wallmaterial
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);
	ResourceHandler::GetMaterial(MATERIAL::BULKHEADS_STANDARD)->SetCullMode(CULL_MODE_NONE);

	//SetClipPlanes(0);

	// Generate rooms
	m_pWorld->Generate(*this);
}

void SceneGame::CreateCrew() noexcept
{
	//Crew
	std::string names[] = {
		"Granfeldt",
		"Ola",
		"Sven",
		"Gunnar",
		"Fysik-GW",
		"Robban",
		"Bengan",
		"Ragnar",
		"Klasse",
		"Gustafsson",
		"Nisse",
		"Per-Egon",
		"Knut",
		"Britt-Marie",
		"Bert Karlsson",
		"Herman Söderlund"
	};

	int index = 0;
	float x, y, z;
	bool hidden = true;

	Crewmember* crewmember;
	m_Crew.AddMember(m_pWorld, glm::vec3(10.0f, 4.0f, 10.0f), names[0], GroupType::SMOKE_DIVER);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)4.0f).GetLevel()[(int)10.0f][(int)10.0f]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::NONE);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::MEDIC);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::NONE);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::SMOKE_DIVER);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::NONE);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::MEDIC);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
	x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
	z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
	m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[index % NUM_CREW], GroupType::NONE);
	crewmember = m_Crew.GetMember(index++);
	crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
	crewmember->SetHidden(hidden);
	crewmember->UpdateTransform();
	AddGameObject(crewmember);

	for (int i = index; i < NUM_CREW; i++)
	{
		y = (float)((std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2);
		x = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeX() - 2) + 1);
		z = (float)(std::rand() % (m_pWorld->GetLevel((uint32)y).GetSizeZ() - 2) + 1);
		m_Crew.AddMember(m_pWorld, glm::vec3(x, y, z), names[i % NUM_CREW], GroupType::NONE);
		crewmember = m_Crew.GetMember(i);
		crewmember->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
		crewmember->SetHidden(hidden);
		crewmember->UpdateTransform();
		AddGameObject(m_Crew.GetMember(i));
	}
}

void SceneGame::PickPosition()
{
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), Game::GetGame()->GetWindow().GetWidth(), Game::GetGame()->GetWindow().GetHeight());
	glm::vec3 rayOrigin = GetCamera().GetPosition();
	glm::vec3 pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	float t = -1, lastT = -1;
	for (int d = m_pWorld->GetNumLevels() - 2; d >= 0; d -= 2)
	{
		if (glm::dot(normal, rayDir) < -0.01)
		{
			t = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, rayDir);
		}

		if ((t >= 0 && lastT == -1) || (t > 0 && t < lastT))
		{
			pointOnSurface = rayOrigin + rayDir * t;

			float extension = GetExtension();
			if (pointOnSurface.x > extension * d / 2.0f && (glm::floor(pointOnSurface.y / 2) == glm::floor(GetCamera().GetLookAt().y / 2) || extension > 0))
			{
				pointOnSurface.x -= extension * floor(pointOnSurface.y / 2.0f);
				lastT = t;
			}
			else
			{
				pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	if (pointOnSurface != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		const std::vector<int32>& selectedList = m_Crew.GetSelectedList();
		for (int32 i = 0; i < selectedList.size(); i++)
		{
			glm::ivec3 tile = glm::round(pointOnSurface);
			if (i > 0)
			{
				tile.x = glm::clamp(tile.x, 1, 10);
				tile.y = glm::clamp(tile.y, 0, 4);
				tile.z = glm::clamp(tile.z, 1, 40);

				WorldLevel& level = m_pWorld->GetLevel(tile.y);
				if (level.GetLevel()[tile.x + 1][tile.z] == level.GetLevel()[tile.x][tile.z] && i < 2)
				{
					tile.x += 1;
				}
				else if (level.GetLevel()[tile.x - 1][tile.z] == level.GetLevel()[tile.x][tile.z] && i < 3)
				{
					tile.x -= 1;
				}
				else if (level.GetLevel()[tile.x][tile.z + 1] == level.GetLevel()[tile.x][tile.z] && i < 4)
				{
					tile.z += 1;
				}
				else if (level.GetLevel()[tile.x][tile.z - 1] == level.GetLevel()[tile.x][tile.z] && i < 5)
				{
					tile.z -= 1;
				}
				else if (level.GetLevel()[tile.x + 1][tile.z + 1] == level.GetLevel()[tile.x][tile.z] && i < 6)
				{
					tile.x += 1;
					tile.z += 1;
				}
				else if (level.GetLevel()[tile.x - 1][tile.z - 1] == level.GetLevel()[tile.x][tile.z] && i < 7)
				{
					tile.x -= 1;
					tile.z -= 1;
				}
				else if (level.GetLevel()[tile.x - 1][tile.z + 1] == level.GetLevel()[tile.x][tile.z] && i < 8)
				{
					tile.x -= 1;
					tile.z += 1;
				}
				else if (level.GetLevel()[tile.x + 1][tile.z - 1] == level.GetLevel()[tile.x][tile.z] && i < 9)
				{
					tile.x += 1;
					tile.z -= 1;
				}
			}

			m_Crew.GetMember(selectedList[i])->FindPath(tile);
		}
	}
}

void SceneGame::RequestDoorClosed(uint32 doorColor)
{
	for (uint32 i = 0; i < m_Crew.GetCount(); i++)
	{
		Crewmember* pCrewmember = m_Crew.GetMember(i);
		pCrewmember->SetCloseColor(doorColor);
		pCrewmember->LookForDoor();
	}
}

void SceneGame::SetUIVisible(bool visible) noexcept
{
	m_pUICrewMember->SetVisible(visible);
	m_pUIRequest->SetVisible(visible);
	m_pUILog->SetVisible(visible);

	m_pUICrew->SetVisible(visible);
	m_pUINotification->SetVisible(visible);
}

void SceneGame::Pick(bool hover, int32 positionX, int32 positionY)
{
	GameObject* object = RayTestGameObjects();

	if (object)
	{
		Crewmember* crewMember = dynamic_cast<Crewmember*>(object);

		if (crewMember)
		{
			if (hover)
			{
				if (!object->IsHovered())
				{
					object->OnHovered();
				}
			}
			else if(!Input::IsKeyDown(KEY_LEFT_CTRL))
			{
				if (((Crewmember*)object)->IsAbleToWalk())
				{
					m_Crew.ClearSelectedList();
				}
				object->OnPicked(m_Crew.GetSelectedList(), positionX, positionY);
			}
			else
			{
				object->OnPicked(m_Crew.GetSelectedList(), positionX, positionY);
			}
		}
		else if (m_Crew.HasSelectedMembers() && !hover)
		{
			object->OnPicked(m_Crew.GetSelectedList(), positionX, positionY);
		}
	}
	else if (hover)
	{
		for (int i = 0; i < m_PickableGameObjects.size(); i++)
		{
			
			if (m_PickableGameObjects[i]->IsHovered())
			{
				m_PickableGameObjects[i]->OnNotHovered();
			}
		}
	}
	else
	{
		m_Crew.ClearSelectedList();
		m_pUICrew->Deselect();
	}
}

glm::vec3 SceneGame::GetRay(const glm::vec2 & mousepos, uint32 windowWidth, uint32 windowHeight)
{
	glm::vec4 rayDir4((2.0f * mousepos.x) / windowWidth - 1.0f, 1.0f - (2.0f * mousepos.y) / windowHeight, -1.0, 1.0);
	rayDir4 = GetCamera().GetInverseProjectionMatrix() * rayDir4;
	rayDir4 = glm::vec4(glm::vec2(rayDir4), -1.0, 0.0);
	rayDir4 = GetCamera().GetInverseViewMatrix() * rayDir4;

	glm::vec3 rayDir = glm::normalize(glm::vec3(rayDir4));

	return rayDir;
}

void SceneGame::ShowCrewmember(uint32 crewmember)
{
	m_Crew.GetMember(crewmember)->ReportPosition();
}

GameObject* SceneGame::RayTestGameObjects()
{
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), Game::GetGame()->GetWindow().GetWidth(), Game::GetGame()->GetWindow().GetHeight());
	glm::vec3 rayOrigin = GetCamera().GetPosition();
	float elevation = GetCamera().GetLookAt().y;

	float lastT = -1;
	uint32 id = -1;

	for (int i = 0; i < (int32)m_PickableGameObjects.size(); i++)
	{
		int32 t = m_PickableGameObjects[i]->TestAgainstRay(rayDir, rayOrigin, elevation, GetExtension());
		if (t > 0 && lastT == -1 || t >= 0 && t < lastT)
		{
			lastT = t;
			id = i;
		}
	}

	if (id != -1)
	{
		return m_PickableGameObjects[id];
	}
	return nullptr;
}

Crewmember* SceneGame::GetCrewmember(uint32 shipNumber)
{
	return m_Crew.GetMember(shipNumber);
}

Crew* SceneGame::GetCrew() noexcept
{
	return &m_Crew;
}

UICrewMember* SceneGame::GetUICrewMember() noexcept
{
	return m_pUICrewMember;
}

UINotification * SceneGame::GetUINotification() noexcept
{
	return m_pUINotification;
}

World* SceneGame::GetWorld() noexcept
{
	return m_pWorld;
}

void SceneGame::UpdateCamera(float dtS) noexcept
{
	GetCamera().UpdateFromLookAt();

	glm::vec3 newPosition(GetCamera().GetLookAt() + glm::vec3(0.0f, 0.06f, 0.0f));
	newPosition.x -= GetExtension() * glm::floor(newPosition.y / 2);
	m_pLookAt->SetPosition(newPosition);
	m_pLookAt->UpdateTransform();
}

void SceneGame::SetPaused(bool paused) noexcept
{
	m_IsPaused = paused;
}

bool SceneGame::IsPaused() const noexcept
{
	return m_pUIPause;
}
