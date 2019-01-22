#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoords;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 model;
	vec4 clipPlane;
};

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
};

out VS_OUT
{
	vec3 Position;
	vec3 Normal;
} vs_out;

void main()
{
	vec4 worldPos = model * vec4(position, 1.0);
	vs_out.Position = worldPos.xyz;
	vs_out.Normal = (model * vec4(normal, 0.0)).xyz;
	gl_Position = cameraCombined * worldPos;

}