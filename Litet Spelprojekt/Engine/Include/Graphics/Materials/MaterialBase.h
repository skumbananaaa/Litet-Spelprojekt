#pragma once
#include <EnginePch.h>

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS MAX_ROOMS_VISIBLE
#define NUM_SPOT_LIGHTS 2

#define LEVEL_SIZE_X 12
#define LEVEL_SIZE_Y 6
#define LEVEL_SIZE_Z 42

#define DIFFUSE_MAP_BINDING_SLOT 0
#define SHADOW_MAP_0_BINDING_SLOT 1
#define SHADOW_MAP_1_BINDING_SLOT 2
#define SHADOW_MAP_2_BINDING_SLOT 3
#define SHADOW_MAP_3_BINDING_SLOT 4
#define SHADOW_MAP_4_BINDING_SLOT 5

#define CAMERA_BUFFER_BINDING_SLOT 0
#define LIGHT_BUFFER_BINDING_SLOT 1
#define MATERIAL_BUFFER_BINDING_SLOT 2
#define WORLD_BUFFER_BINDING_SLOT 3
#define PLANE_BUFFER_BINDING_SLOT 4
#define EXTENSION_BUFFER_BINDING_SLOT 5
#define WATER_BUFFER_BINDING_SLOT 6
#define ANIMATION_BUFFER_BINDING_SLOT 7
#define SHADOW_BUFFER_BINDING_SLOT 8

__declspec(align(16)) struct DirectionalLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Direction = glm::vec3(0.0f);
};

__declspec(align(16)) struct PointLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Position = glm::vec3(0.0f);
};

__declspec(align(16)) struct SpotLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Position = glm::vec3(0.0f);
	float CutOffAngle = 1.0f;
	glm::vec3 Direction = glm::vec3(0.0f);
	float OuterCutOffAngle = 1.0f;
};

struct CameraBuffer
{
	glm::mat4 ProjectionView;
	glm::mat4 View;
	glm::mat4 Projection;
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
	glm::vec3 CameraLookAt;
	float pad1;
	glm::vec3 CameraPosition;
};

struct LightBuffer
{
	DirectionalLightBuffer DirectionalLights[NUM_DIRECTIONAL_LIGHTS];
	PointLightBuffer PointLights[NUM_POINT_LIGHTS];
	SpotLightBuffer SpotLights[NUM_SPOT_LIGHTS];
};

struct MaterialBuffer
{
	glm::vec4 Color;
	glm::vec4 ClipPlane;
	float Specular;
	float HasDiffuseMap;
};

struct WorldBuffer
{
	uint32 map[LEVEL_SIZE_X * LEVEL_SIZE_Y * LEVEL_SIZE_Z];
};

struct PlaneBuffer
{
	glm::vec4 ClipPlane;
};

struct ShadowBuffer
{
	glm::vec4 LightPosition[NUM_POINT_LIGHTS];
};