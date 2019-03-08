#include <EnginePch.h>
#include <World\LightManager.h>
#include <Graphics/Scene.h>
#include <Graphics/Lights/SpotLight.h>

std::vector<SpotLight*> LightManager::s_Spotlights;
std::vector<SpotLight*> LightManager::s_SpotlightsInUse;
Scene* LightManager::s_Scene;

void LightManager::Init(Scene* scene, int32 nrOfLights) noexcept
{
	s_Scene = scene;
	for (int i = 0; i < nrOfLights; i++)
	{
		s_Spotlights.push_back(new SpotLight(glm::vec3(), 1.0, 1.0, glm::vec3(1, 0, 0)));
	}
}

void LightManager::Release() noexcept
{
	for (SpotLight* spotlight : s_Spotlights)
	{
		bool isFree = true;
		for (SpotLight* object : s_SpotlightsInUse)
		{
			if (spotlight == object)
			{
				isFree = false;
				break;
			}
		}
		if (isFree)
		{
			Delete(spotlight);
		}
	}
	s_Spotlights.clear();
	s_SpotlightsInUse.clear();
}

SpotLight* LightManager::AcquireSpotlight(const glm::vec3& pos, float innerAngle, float outerAngle, const glm::vec3& direction, const glm::vec4& color) noexcept
{
	if (s_SpotlightsInUse.size() == s_Spotlights.size())
	{
		return nullptr;
	}
	SpotLight* result = nullptr;
		
	for (SpotLight* spotlight : s_Spotlights)
	{
		bool isFree = true;
		for (SpotLight* object : s_SpotlightsInUse)
		{
			if (spotlight == object)
			{
				isFree = false;
				break;
			}
		}
		if (isFree)
		{
			result = spotlight;
			break;
		}
	}
	s_SpotlightsInUse.push_back(result);
	s_Scene->AddSpotLight(result);

	result->SetPosition(pos);
	result->SetCutOffAngle(innerAngle);
	result->SetOuterCutOffAngle(outerAngle);
	result->SetDirection(direction);
	result->SetColor(color);
	result->UpdateTransform();
	return result;
}

void LightManager::ReturnSpotlight(SpotLight* spotlight)
{
	int32 counter = 0;
	for (SpotLight* object : s_SpotlightsInUse)
	{
		if (spotlight == object)
		{
			s_SpotlightsInUse.erase(s_SpotlightsInUse.begin() + counter);
			break;
		}
		counter++;
	}

	s_Scene->RemoveSpotLight(spotlight);
}
