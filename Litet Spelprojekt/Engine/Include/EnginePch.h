#pragma once

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

static const glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);