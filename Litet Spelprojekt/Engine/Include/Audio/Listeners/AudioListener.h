#pragma once
#include <EnginePch.h>
#include <GLM/glm.hpp>

class API AudioListener
{
public:
	static void SetPosition(const glm::vec3& pos) noexcept;
	static void SetOrientation(const glm::vec3& lookAt, const glm::vec3& up) noexcept;
	static void SetMasterVolume(float vol) noexcept;
};