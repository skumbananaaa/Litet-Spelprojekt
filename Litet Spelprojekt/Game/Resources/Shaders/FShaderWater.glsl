#version 420 core

in VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} fs_in;

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
	float distortionMoveFactor;
	vec4 clipPlane;
};

layout (binding = 0) uniform sampler2D reflectionTexture;
layout (binding = 1) uniform sampler2D refractionTexture;
layout (binding = 2) uniform sampler2D dudvMap;

out vec4 FragColor;

const float distortionStrength = 0.005;

void main()
{
	vec2 ndcTexCoords = (fs_in.ClipSpacePosition.xy / fs_in.ClipSpacePosition.w) / 2.0 + 0.5;

	vec2 distortion1 = (texture(dudvMap, vec2(fs_in.TexCoords.x + distortionMoveFactor, fs_in.TexCoords.y)).rg * 2.0 - 1.0) * distortionStrength;
	vec2 distortion2 = (texture(dudvMap, vec2(1.0 - fs_in.TexCoords.x, fs_in.TexCoords.y + distortionMoveFactor)).rg * 2.0 - 1.0) * distortionStrength;
	vec2 totalDistortion = distortion1 + distortion2;

	vec2 reflectionTexCoords = vec2(ndcTexCoords.x, -ndcTexCoords.y);
	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999); //Fixar wobbly kant
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec2 refractionTexCoords = vec2(ndcTexCoords.x, ndcTexCoords.y);
	refractionTexCoords += totalDistortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

	vec4 reflectionColor = texture(reflectionTexture, reflectionTexCoords);
	vec4 refractionColor = texture(refractionTexture, refractionTexCoords);

	FragColor = mix(reflectionColor, refractionColor, 0.5);
	FragColor = mix(FragColor, vec4(0.14, 0.51, 0.67, 1.0), 0.25);
}