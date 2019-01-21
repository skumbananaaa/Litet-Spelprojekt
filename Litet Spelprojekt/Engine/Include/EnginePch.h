#pragma once

#if defined(ENGINE_EXPORT)
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#endif

#include <GLM\glm.hpp>
#include "Defines.h"

#include <cassert>
#include <iostream>
#include <Fstream>
#include <sstream>

#include <chrono>
#include <string>
#include <vector>

static const glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);