#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <World/Grid.h>

#if defined(_DEBUG)
//#define DRAW_DEBUG_BOXES
#endif

#define CHOSEN glm::vec4(0.0f, 0.1f, 0.0f, 1.0f)

GameObject* g_pDecalObject = nullptr;

float g_Rot = 1.0;

Game::Game() noexcept : 
	Application(false),
	m_pRenderer(nullptr),
	m_pDebugRenderer(nullptr),
	m_pScene(nullptr),
	m_pSkyBoxTex(nullptr),
	m_pWorld(nullptr),
	m_pSoundEffect(nullptr),
	m_pTextViewFPS(nullptr),
	m_pTextViewUPS(nullptr),
	m_pMusic(nullptr),
	m_pTestAudioSource(nullptr),
	cartesianCamera(true),
	m_CurrentElevation(2)
{
	m_pScene = new Scene();
	ResourceHandler::LoadResources(this);

	m_pRenderer = new DefferedRenderer();
	m_pDebugRenderer = new DebugRenderer();

	m_pSkyBoxTex = new TextureCube(ResourceHandler::GetTexture2D(TEXTURE::HDR));
	m_pScene->SetSkyBox(new SkyBox(m_pSkyBoxTex));

	//Lights
	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec3(0.0f, 0.5f, 0.5f));
	m_pScene->AddDirectionalLight(pDirectionalLight);

	m_pScene->AddPointLight(new PointLight(glm::vec3(5.0f, 2.0f, -10.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(2.0f, 2.0f, -10.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(-5.0f, 2.0f, -10.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

	m_pScene->AddSpotLight(new SpotLight(glm::vec3(1.0f, 3.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.5f)), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.5f, 1.0f)));

	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	GetGUIManager().Add(m_pTextViewFPS);
	GetGUIManager().Add(m_pTextViewUPS);

	//Audio
	//m_pSoundEffect = new SoundEffect("Resources/Audio/Stereo/Seagulls.wav");
	m_pSoundEffect = new SoundEffect("Resources/Audio/Mono/fart.wav");
	m_pMusic = new Music("Resources/Audio/Music/WavesAndSeagulls.ogg");
	m_pTestAudioSource = new AudioSource(*m_pMusic);
	m_pTestAudioSource->SetPitch(1.0f);
	m_pTestAudioSource->SetLooping(true);
	m_pTestAudioSource->Play();

	AudioListener::SetPosition(glm::vec3(0.0f));
}

Game::~Game()
{
	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pDebugRenderer);

	DeleteSafe(m_pSkyBoxTex);

	DeleteSafe(m_pScene);
	
	DeleteSafe(m_pTextViewFPS);
	DeleteSafe(m_pTextViewUPS);
	
	DeleteSafe(m_pSoundEffect);
	DeleteSafe(m_pMusic);
	
	DeleteSafe(m_pTestAudioSource);
	DeleteSafe(m_pWorld);

	ResourceHandler::ReleaseResources();
}

void Game::OnResourcesLoaded()
{
	Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	pCamera->CreatePerspective(glm::radians<float>(90.0f), GetWindow().GetAspectRatio(), 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	GameObject* pGameObject = nullptr;
	pGameObject = new GameObject();
	pGameObject->SetDecal(DECAL::BLOOD);
	pGameObject->SetPosition(glm::vec3(-6.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(3.0f, 4.0f, 3.0f));
	pGameObject->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	pGameObject->UpdateTransform();
	g_pDecalObject = pGameObject;
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetName("ship");
	pGameObject->SetMaterial(MATERIAL::BOAT);
	pGameObject->SetMesh(MESH::SHIP);
	pGameObject->SetPosition(glm::vec3(5.5f, -3.0f, 12.5f));
	pGameObject->SetScale(glm::vec3(1.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);


	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::GROUND);
	pGameObject->SetMesh(MESH::CLIFF_3_LOW);
	pGameObject->SetPosition(glm::vec3(0.0f, -1.4f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.4f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::RED);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(5.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::GREEN);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(2.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::BLUE);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(-5.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::BLUE);
	pGameObject->SetMesh(MESH::CUBE_INV_NORMALS);
	pGameObject->SetPosition(pCamera->GetLookAt());
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	pGameObject->SetName("cameraLookAt");
	m_pScene->AddGameObject(pGameObject);

	//Water?? YAAAS
	pGameObject = new GameObject();
	pGameObject->SetIsReflectable(true);
	pGameObject->SetMesh(MESH::QUAD);
	pGameObject->SetScale(glm::vec3(200.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	m_pWorld = WorldSerializer::Read("world.json");

	//Enable clipplane for wallmaterial
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->EnableClipPlane(true);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);

	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->EnableClipPlane(true);
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)->EnableClipPlane(true);
	SetClipPlanes();

	for (int level = 0; level < m_pWorld->GetNumLevels(); level += 2) 
	{
		m_pWorld->GenerateWalls(level);
		glm::vec4 wall;

		for (int i = 0; i < m_pWorld->GetLevel(level)->GetNrOfWalls(); i++)
		{
			wall = m_pWorld->GetLevel(level)->GetWall(i);
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::WALL_STANDARD);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetPosition(glm::vec3(wall.x, 1.0f + level, wall.y));
			pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
			pGameObject->UpdateTransform();
			
			m_pScene->AddGameObject(pGameObject);
		}
	}

	float x, y, z;
	for (int i = 0; i < 15; i++)
	{
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y)->GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y)->GetSizeZ() - 2) + 1;
		m_Crew.AddMember(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec3(x, 0.9f + y, z));
		m_pScene->AddGameObject(m_Crew.GetMember(i));
		m_pScene->AddPointLight(m_Crew.GetMember(i)->GetLight());
		m_Crew.GetMember(i)->SetPath(m_pWorld);
		m_Crew.GetMember(i)->UpdateTransform();
	}
}

void Game::OnKeyUp(KEY keycode)
{
	Application::OnKeyUp(keycode);
}

void Game::OnKeyDown(KEY keycode)
{
	switch (keycode)
	{
		case KEY_O:
		{
			cartesianCamera = !cartesianCamera;
			break;
		}
		case KEY_P:
		{
			m_pTestAudioSource->TogglePause();
			break;
		}
	}

	Application::OnKeyDown(keycode);
}

void Game::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	
}

void Game::OnMouseReleased(MouseButton mousebutton, const glm::vec2 & position)
{
	switch (mousebutton)
	{
		case MOUSE_BUTTON_LEFT:
		{
			this->PickPosition();
			break;
		}
		case MOUSE_BUTTON_RIGHT:
		{
			this->PickCrew();
			break;
		}
	}
}

void Game::OnUpdate(float dtS)
{
	m_pScene->OnUpdate(dtS);

	if (cartesianCamera)
	{
		//Cartesian
		static float cartesianCameraSpeed = 5.0f;
		static float cartesianCameraAngularSpeed = 1.5f;

		if (Input::IsKeyDown(KEY_W))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Forward, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Backwards, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Left, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Right, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Up, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Down, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pScene->GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pScene->GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pScene->GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pScene->GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
		}

		m_pScene->GetCamera().UpdateFromPitchYaw();
	}
	else
	{
		//Polar
		static float polarCameraSpeed = 5.0f;
		static float polarCameraAngularSpeed = 0.8f;

		if (Input::IsKeyDown(KEY_W))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Forward, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Backwards, polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Left, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Right, polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Up, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Down, polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateUp, polarCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateDown, polarCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateLeft, polarCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateRight, polarCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_X))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::ZoomIn, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Z))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::ZoomOut, polarCameraSpeed * dtS);
		}

		m_pScene->GetCamera().UpdateFromLookAt();
	}

	m_pScene->GetGameObject("cameraLookAt")->SetPosition(m_pScene->GetCamera().GetLookAt());

	m_pTextViewFPS->SetText("FPS " + std::to_string(GetFPS()));
	m_pTextViewUPS->SetText("UPS " + std::to_string(GetUPS()));

	AudioListener::SetPosition(m_pScene->GetCamera().GetPosition());
	AudioListener::SetOrientation(m_pScene->GetCamera().GetFront(), m_pScene->GetCamera().GetUp());

	static float decalRot = 0.0f;
	static float decalX = g_pDecalObject->GetPosition().x;
	static float decalXSpeed = -1.0f;
	
	if (decalX > 6.5f)
	{
		decalXSpeed = -1.0f;
	}
	else if (decalX < -6.5f)
	{
		decalXSpeed = 1.0f;
	}

	decalX += decalXSpeed * dtS;
	//decalRot += (glm::half_pi<float>() / 2.0f) * dtS;

	g_pDecalObject->SetRotation(glm::vec4(0.0f, 0.0f, 1.0f, glm::radians<float>(-45.0f)));
	g_pDecalObject->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	g_pDecalObject->UpdateTransform();
	if (Input::IsKeyPressed(KEY_NUMPAD_2))
	{
		if (m_CurrentElevation > 0)
		{
			m_CurrentElevation--;
		}
		
		SetClipPlanes();
		std::cout << "Elevation: " << m_CurrentElevation << std::endl;
	}

	if (Input::IsKeyPressed(KEY_NUMPAD_8))
	{
		if (m_CurrentElevation < 2)
		{
			m_CurrentElevation++;
		}

		SetClipPlanes(); 
		std::cout << "Elevation: " << m_CurrentElevation << std::endl;
	}
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pScene, dtS);
	
#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_pScene);
#endif
}

void Game::PickPosition() {
	glm::vec3 rayDir = this->GetRay(Input::GetMousePosition(), this->GetWindow().GetWidth(), this->GetWindow().GetHeight());
	glm::vec3 rayOrigin = m_pScene->GetCamera().GetPosition();
	glm::vec3 pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);
	
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	float t = -1, lastT = -1;
	for (int d = 0; d < m_pWorld->GetNumLevels(); d += 2)
	{
		if (glm::dot(normal, rayDir) < -0.01)
		{
			t = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, rayDir);
		}

		if ((t >= 0 && lastT == -1) || (t > 0 && t < lastT))
		{
			pointOnSurface = rayOrigin + rayDir * t;
		}
	}

	if (pointOnSurface != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		for (int i = 0; i < m_Crew.GetCount(); i++)
		{
			if (m_Crew.GetMember(i)->GetLight()->GetColor() == CHOSEN)
			{
				m_Crew.GetMember(i)->FindPath(glm::round(pointOnSurface));
			}
		}
	}
}

void Game::PickCrew()
{
	glm::vec3 rayDir = this->GetRay(Input::GetMousePosition(), this->GetWindow().GetWidth(), this->GetWindow().GetHeight());
	glm::vec3 rayOrigin = m_pScene->GetCamera().GetPosition();

	float lastT = -1;
	uint32 id = -1;

	for (int i = 0; i < m_Crew.GetCount(); i++)
	{
		glm::vec3 centre = m_Crew.GetMember(i)->GetPosition();

		glm::vec3 normals[]{
			m_Crew.GetMember(i)->GetDirection(),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::normalize(glm::cross(normals[0], normals[1]))
		};

		float h[] = {
			0.1,
			0.9,
			0.25
		};

		float d1[] = {
			glm::dot(centre - normals[0] * h[0], normals[0]),
			glm::dot(centre - normals[1] * h[1], normals[1]),
			glm::dot(centre - normals[2] * h[2], normals[2])
		};
		float d2[] = {
			glm::dot(centre + normals[0] * h[0], normals[0]),
			glm::dot(centre + normals[1] * h[1], normals[1]),
			glm::dot(centre + normals[2] * h[2], normals[2])
		};

		float t1[3];
		float t2[3];
		float t_min[3];
		float t_max[3];

		float t = -1;
		float min_t, max_t;

		for (int j = 0; j < 3; j++)
		{
			if (std::abs(glm::dot(normals[j], rayDir)) > 0.01)
			{
				t1[j] = (d1[j] - glm::dot(normals[j], rayOrigin)) / glm::dot(normals[j], rayDir);
				t2[j] = (d2[j] - glm::dot(normals[j], rayOrigin)) / glm::dot(normals[j], rayDir);

				t_min[j] = std::min(t1[j], t2[j]);
				t_max[j] = std::max(t1[j], t2[j]);
			}
			else if (-glm::dot(normals[0], centre - rayOrigin) - h[j] > 0 || -glm::dot(normals[0], centre - rayOrigin) + h[j] < 0)
				return;
		}

		min_t = std::max(t_min[0], t_min[1]);
		min_t = std::max(min_t, t_min[2]);
		max_t = std::min(t_max[0], t_max[1]);
		max_t = std::min(max_t, t_max[2]);

		if (min_t <= max_t && max_t >= 0)
		{
			if (t_min > 0)
				t = min_t;
			else
				t = max_t;
		}

		if (t > 0 && lastT == -1 || t >= 0 && t < lastT)
		{
			lastT = t;
			id = i;
		}
	}

	if (id != -1)
	{
		if (m_Crew.GetMember(id)->GetLight()->GetColor() == CHOSEN)
		{
			m_Crew.GetMember(id)->GetLight()->SetColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		}
		else
		{
			m_Crew.GetMember(id)->GetLight()->SetColor(CHOSEN);
		}
	}
}

glm::vec3 Game::GetRay(const glm::vec2 & mousepos, uint32 windowWidth, uint32 windowHeight)
{
	glm::vec4 rayDir4((2.0f * mousepos.x) / windowWidth - 1.0f, 1.0f - (2.0f * mousepos.y) / windowHeight, -1.0, 1.0);
	rayDir4 = m_pScene->GetCamera().GetInverseProjectionMatrix() * rayDir4;
	rayDir4 = glm::vec4(glm::vec2(rayDir4), -1.0, 0.0);
	rayDir4 = m_pScene->GetCamera().GetInverseViewMatrix() * rayDir4;

	glm::vec3 rayDir = glm::normalize(glm::vec3(rayDir4));

	return rayDir;
}


void Game::SetClipPlanes()
{
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 1.8f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));
}
