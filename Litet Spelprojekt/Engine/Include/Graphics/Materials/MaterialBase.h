#pragma once
#include <EnginePch.h>

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 18
#define NUM_SPOT_LIGHTS 17

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
	float HasNormalMap;
	float HasSpecularMap;
};