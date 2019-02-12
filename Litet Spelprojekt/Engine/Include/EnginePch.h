#pragma once

#define GLM_FORCE_SSE2
#include <GLM/glm.hpp>
#if defined(ENGINE_EXPORT)
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include "Defines.h"
#include "Types.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

static const glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
static const glm::vec3 FORWARD_VECTOR(0.0f, 0.0f, 1.0f);
static const glm::vec3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);