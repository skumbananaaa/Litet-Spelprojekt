#pragma once
#include <EnginePch.h>

class Scene;
class SpotLight;

class API LightManager
{
public:
	static void Init(Scene* scene, int32 nrOfLights) noexcept;
	static SpotLight* AcquireSpotlight(const glm::vec3& pos, float innerAngle, float outerAngle, const glm::vec3& direction, const glm::vec4& color = glm::vec4(1.0f)) noexcept;
	static void ReturnSpotlight(SpotLight* spotlight);

private:
	LightManager();
	static Scene* s_Scene;
	static std::vector<SpotLight*> s_Spotlights;
	static std::vector<SpotLight*> s_SpotlightsInUse;
};