#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoords;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 model;
};

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
	float distortionMoveFactor;
};

out VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} vs_out;

const float tiling = 4.0; //Hur fan funkar detta?

void main()
{
	vec4 worldPos = model * vec4(position, 1.0);

	vs_out.Position = worldPos.xyz;
	vs_out.Normal = (model * vec4(normal, 0.0)).xyz;
	vs_out.TexCoords = texCoords * tiling;
	vs_out.ClipSpacePosition = cameraCombined * worldPos;
	gl_Position = vs_out.ClipSpacePosition;
}