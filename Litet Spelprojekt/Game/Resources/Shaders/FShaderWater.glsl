#version 420 core

in VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
	float padding;
	vec4 clipPlane;
};

layout (binding = 2) uniform sampler2D reflectionTexture;
layout (binding = 3) uniform sampler2D refractionTexture;

out vec4 FragColor;

void main()
{
	vec4 reflectionColor = texture(reflectionTexture, fs_in.TexCoords);
	vec4 refractionColor = texture(refractionTexture, fs_in.TexCoords);

	FragColor = mix(reflectionColor, refractionColor, 0.5);
}