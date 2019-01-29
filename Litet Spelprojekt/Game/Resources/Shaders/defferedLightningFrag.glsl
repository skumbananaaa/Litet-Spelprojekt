#version 420

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 8

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2DMS g_Color;
layout(binding = 1) uniform sampler2DMS g_Normal;
layout(binding = 2) uniform sampler2DMS g_Depth;

struct DirectionalLight
{
	vec4 Color;
	vec4 Direction;
};

struct PointLight
{
	vec4 Color;
	vec4 Position;
};

layout(binding = 0) uniform LightPassBuffer
{
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec4 g_CameraPosition;
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
};

vec3 PositionFromDepth(float depth)
{
	float z = (depth * 2.0f) - 1.0f;

	vec4 posClipSpace = vec4((fs_in.TexCoords * 2.0f) - vec2(1.0f), z, 1.0f);
	vec4 posViewSpace = g_InverseProjection * posClipSpace;

	posViewSpace = posViewSpace / posViewSpace.w;
	vec4 worldPosition = g_InverseView * posViewSpace;
	return worldPosition.xyz;
}

vec3 NormalDecode(vec3 mappedNormal)
{
	return (mappedNormal * 2.0f) - vec3(1.0f);
}

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 viewDir, vec3 normal, vec3 color)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//AMBIENT
	vec3 ambient = vec3(0.1f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f));

	//SPECULAR
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);
	vec3 specular = vec3(spec) * lightColor;

	return ((ambient + diffuse) * color * lightColor) + specular;
}

vec4 sampleMSAATexture(sampler2DMS tex, vec2 nTexCoords)
{
	ivec2 texSize = textureSize(tex, 0);
	ivec2 texCoord = ivec2(nTexCoords) * texSize;

	vec4 color = vec4(0.0f);
	for (int i = 0; i < 2; i++)
	{
		color += texelFetch(tex, texCoord, i);
	}

	return color;
}

void main()
{
	float depth = sampleMSAATexture(g_Depth, fs_in.TexCoords).r;
	vec3 normal = normalize(sampleMSAATexture(g_Normal, fs_in.TexCoords).xyz);
	vec3 position = PositionFromDepth(depth);

	vec3 color = sampleMSAATexture(g_Color, fs_in.TexCoords).rgb;
	vec3 viewDir = normalize(g_CameraPosition.xyz - position);
	
	//Do  lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;

		c += CalcLight(lightDir, lightColor, viewDir, normal, color);
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		vec3 lightDir = g_PointLights[i].Position.xyz - position;
		float distance = length(lightDir);
		lightDir = normalize(lightDir);

		float attenuation = 1.0f / (distance * distance);
		vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;

		c += CalcLight(lightDir, lightColor, viewDir, normal, color);
	}

	g_OutColor = vec4(min(c, vec3(1.0f)), 1.0f);
}