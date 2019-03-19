#pragma once
#pragma warning(disable : 4100) //Disable warning about unused parameters
#pragma warning(disable : 4127) //Disable constant expression warning
#pragma warning(disable : 4201) //Disable warning about nonstandard extension used
#pragma warning(disable : 4251) //Disable DLL interface warning for STL
#pragma warning(disable : 4324) //Disable warning about padding
#pragma warning(disable : 4505) //Disable warning about unreferenced function removed (stb_image)
#pragma warning(error : 4005) //Make macro redefinition an error
#pragma warning(error : 4456) //Make redefinition an error
#pragma warning(error : 4458) //Make redefinition an error

#define GLM_FORCE_SSE2
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/norm.hpp>

#if defined(ENGINE_EXPORT)
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <unordered_map>

#include <algorithm>
#include <cmath>
#include <ctime>

#include <crtdbg.h>

static const glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
static const glm::vec3 FORWARD_VECTOR(0.0f, 0.0f, 1.0f);
static const glm::vec3 RIGHT_VECTOR(1.0f, 0.0f, 0.0f);