#pragma once
#include <EnginePch.h>

inline float LengthSqrd(const glm::vec3& vector)
{
	return (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);
}