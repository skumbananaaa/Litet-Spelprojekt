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
layout (binding = 3) uniform sampler2D normalMap;

out vec4 FragColor;

const float distortionStrength = 0.005;

void main()
{
	vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec2 ndcTexCoords = (fs_in.ClipSpacePosition.xy / fs_in.ClipSpacePosition.w) / 2.0 + 0.5;

	vec2 distortionTexCoords = texture(dudvMap, vec2(fs_in.TexCoords.x + distortionMoveFactor, fs_in.TexCoords.y)).rg * 0.1;
	distortionTexCoords = fs_in.TexCoords + vec2(distortionTexCoords.x, distortionTexCoords.y + distortionMoveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortionTexCoords).rg * 2.0 - 1.0) * distortionStrength;


	vec2 reflectionTexCoords = vec2(ndcTexCoords.x, -ndcTexCoords.y);
	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999); //Fixar wobbly kant
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec2 refractionTexCoords = vec2(ndcTexCoords.x, ndcTexCoords.y);
	refractionTexCoords += totalDistortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

	vec4 reflectionColor = texture(reflectionTexture, reflectionTexCoords);
	vec4 refractionColor = texture(refractionTexture, refractionTexCoords);

	vec3 viewDir = normalize(cameraPosition - fs_in.Position);
	float refractionFactor = pow(dot(viewDir, vec3(0.0, 1.0, 0.0)), 6.0);

	vec3 normal = texture(normalMap, distortionTexCoords).xyz;
	normal = normalize(vec3(normal.x * 2.0 - 1.0, normal.y, normal.z * 2.0 - 1.0));

	//Specular
	float specularStrength = 0.6;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	vec3 specular = specularStrength * spec * lightColor;

	FragColor = mix(reflectionColor, refractionColor, refractionFactor);
	FragColor = mix(FragColor, vec4(0.7, 0.25, 0.33, 1.0), 0.25) + vec4(specular, 0.0);
}