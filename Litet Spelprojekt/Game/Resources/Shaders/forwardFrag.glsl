#version 420

#define NUM_CLIP_DISTANCES 8
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 18
#define NUM_SPOT_LIGHTS 8

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec3 WorldPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Diffuse;
layout(binding = 1) uniform sampler2D g_NormalMap;

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

struct SpotLight
{
	vec4 Color;
	vec4 Position;
	vec3 TargetDirection;
	float Angle;
	float OuterAngle;
};

layout(std140, binding = 0) uniform PerFrame
{
	mat4 g_ViewProjection;
	vec3 g_CameraPosition;
	float g_Padding;
	vec3 g_CameraLookAt;
	float g_Padding2;
	vec4 g_ClipDistances[NUM_CLIP_DISTANCES];
};

layout(std140, binding = 1) uniform PerObject
{
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

//layout(std140, binding = 0) uniform PerFrame
//{
//	mat4 g_ViewProjection;
//	vec3 g_CameraPosition;
//	float g_Padding;
//	vec3 g_CameraLookAt;
//};
//
//layout(std140, binding = 1) uniform PerObject
//{
//	vec4 g_Color;
//	float g_HasTexture;
//	float g_HasNormalMap;
//};

layout(binding = 2) uniform LightBuffer
{
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
};

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 viewDir, vec3 normal, vec3 color, float intensity)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//AMBIENT
	vec3 ambient = vec3(0.1f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f)) * intensity;

	//SPECULAR
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);
	vec3 specular = vec3(spec) * lightColor * intensity;

	return ((ambient + diffuse) * color * lightColor) + specular;
}

vec3 SampleNormalMap()
{
	vec3 sampledNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	sampledNormal = tbn * sampledNormal;

	return normalize((sampledNormal * g_HasNormalMap) + (fs_in.Normal * (1.0f - g_HasNormalMap)));
}

vec3 SampleDiffuseMap()
{
	vec3 mappedDiffuse = texture(g_Diffuse, fs_in.TexCoords).rgb;
	vec3 color = g_Color.rgb;

	return (mappedDiffuse * g_HasTexture) + (color * (1.0f - g_HasTexture));
}

void main()
{
	vec3 normal = SampleNormalMap();
	vec3 color = SampleDiffuseMap();
	
	vec3 position = fs_in.WorldPosition;
	vec3 viewDir = normalize(g_CameraPosition.xyz - position);

	//Do lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, 1.0f);
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		vec3 lightDir = g_PointLights[i].Position.xyz - position;
		float dist = length(lightDir);
		lightDir = normalize(lightDir);

		float attenuation = 1.0f / (dist * dist);
		vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, 1.0f);
	}

	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		vec3 lightDir = normalize(g_SpotLights[i].Position.xyz - position);
		vec3 targetDir = g_SpotLights[i].TargetDirection - g_SpotLights[i].Position.xyz;
		vec3 lightColor = g_SpotLights[i].Color.rgb;

		float theta = dot(lightDir, normalize(-targetDir));
		float epsilon = g_SpotLights[i].Angle - g_SpotLights[i].OuterAngle;
		float intensity = clamp((theta - g_SpotLights[i].OuterAngle) / epsilon, 0.0, 1.0);

		if(theta > g_SpotLights[i].OuterAngle)
		{
			c += CalcLight(normalize(lightDir), lightColor, viewDir, normal, color, intensity);
		}
	}

	g_OutColor = vec4(min(c, vec3(1.0f)), 1.0f);
}