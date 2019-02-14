#extension GL_ARB_arrays_of_arrays : enable

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 3
#define NUM_SPOT_LIGHTS 2

#define LEVEL_SIZE_X 12
#define LEVEL_SIZE_Y 6
#define LEVEL_SIZE_Z 42

#define LEVEL_SIZE 756

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out float g_OutDepth;

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

struct SpotLight
{
	vec4 Color;
	vec3 Position;
	float Angle;
	vec3 TargetDirection;
	float OuterAngle;
};

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 g_ProjectionView;
	mat4 g_View;
	mat4 g_Projection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec3 g_CameraLookAt;
	float pad1;
	vec3 g_CameraPosition;
};

layout(std140, binding = 1) uniform LightBuffer
{
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
};

layout(std140, binding = 3) uniform WorldBuffer
{
	ivec4 map[LEVEL_SIZE];
	int concealed;
	uint roomId;
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

float SpecularDecode(float mappedSpecular)
{
	return mappedSpecular * 256.0f;
}

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 viewDir, vec3 normal, vec3 color, float specularIntensity, float intensity)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//AMBIENT
	vec3 ambient = vec3(0.0f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f)) * intensity;

	//SPECULAR
	float spec = pow(max(dot(normal, halfwayDir), 0.0), SpecularDecode(specularIntensity));
	vec3 specular = vec3(spec) * lightColor * intensity;

	return ((ambient + diffuse) * color * lightColor) + specular;
}

vec4 SampleMSAATexture(sampler2DMS tex, vec2 nTexCoords)
{
	ivec2 texSize = textureSize(tex);
	ivec2 framebufSize = ivec2(texSize.x * 2, texSize.y);
	ivec2 texCoord = ivec2(nTexCoords * texSize);

	return texelFetch(tex, texCoord, framebufSize.x % 2);
}

void main()
{
	float depth = SampleMSAATexture(g_Depth, fs_in.TexCoords).r;
	g_OutDepth = depth;

	//NORMAL and POSITION
	vec3 normal = normalize(NormalDecode(SampleMSAATexture(g_Normal, fs_in.TexCoords).xyz));
	vec3 position = PositionFromDepth(depth);
	vec3 viewDir = normalize(g_CameraPosition.xyz - position);
	
	//COLOR and SPECULAR
	vec4 mappedColor = SampleMSAATexture(g_Color, fs_in.TexCoords);
	vec3 color = mappedColor.rgb;
	float specular = mappedColor.a;
	
	ivec3 mapPos = ivec3(round(position.x), position.y, round(position.z));
	mapPos.x = clamp(mapPos.x, 0, 11);
	mapPos.y = clamp(mapPos.y, 0, 5);
	mapPos.z = clamp(mapPos.z, 0, 41);

	uint roomIndex[4];

	roomIndex[0] = map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].x;
	roomIndex[1] = map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].y;
	roomIndex[2] = map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].z;
	roomIndex[3] = map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].w;

	//Do lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		if (concealed == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == roomId || position.y >= 5.9f || position.x > 10.5f || position.x < 0.5f || position.z > 40.5f || position.z < 0.5f)
		{
			vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
			vec3 lightColor = g_DirLights[i].Color.rgb;
			float cosTheta = dot(normal, lightDir);

			c += CalcLight(lightDir, lightColor, viewDir, normal, color, specular, 1.0f);
		}
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		ivec3 lightMapPos = ivec3(round(g_PointLights[i].Position.x), g_PointLights[i].Position.y, round(g_PointLights[i].Position.z));
		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
		lightMapPos.z = clamp(lightMapPos.z, 0, 41);

		uint lightRoomIndex[4];

		lightRoomIndex[0] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].x;
		lightRoomIndex[1] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].y;
		lightRoomIndex[2] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].z;
		lightRoomIndex[3] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].w;

		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2)
		{
			vec3 lightDir = g_PointLights[i].Position.xyz - position;
			float dist = length(lightDir);
	
			float attenuation = 1.0f / (dist);
			vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
			lightDir = normalize(lightDir);
			float cosTheta = dot(normal, lightDir);
	
			c += CalcLight(lightDir, lightColor, viewDir, normal, color, specular, 1.0f);
		}
	}

	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		ivec3 lightMapPos = ivec3(round(g_SpotLights[i].Position.x), g_SpotLights[i].Position.y, round(g_SpotLights[i].Position.z));
		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
		lightMapPos.z = clamp(lightMapPos.z, 0, 41);

		uint lightRoomIndex[4];

		lightRoomIndex[0] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].x;
		lightRoomIndex[1] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].y;
		lightRoomIndex[2] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].z;
		lightRoomIndex[3] = map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].w;

		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2)
		{
			float light_attenuation = 1.0f;
			vec3 lightDir = g_SpotLights[i].Position.xyz - position;
			vec3 targetDir = normalize(g_SpotLights[i].TargetDirection);
			float dist = length(lightDir);
			lightDir = normalize(lightDir);
			float cosTheta = dot(normal, lightDir);
	
			float lightToSurfaceAngle = degrees(acos(dot(-lightDir, targetDir)));
			float coneAngle = degrees(acos(g_SpotLights[i].Angle));
			if (lightToSurfaceAngle > coneAngle)
			{
				light_attenuation += lightToSurfaceAngle - coneAngle;
			}
			float attenuation = 1.0f / ((dist));
			
			vec3 lightColor = g_SpotLights[i].Color.rgb * attenuation;
	
			float theta = dot(lightDir, -targetDir);
			float epsilon = g_SpotLights[i].Angle - g_SpotLights[i].OuterAngle;
			float intensity = 10 * clamp((theta - g_SpotLights[i].OuterAngle) / epsilon, 0.0, 1.0);
	
			if(theta > g_SpotLights[i].OuterAngle)
			{
				c += CalcLight(normalize(lightDir), lightColor, viewDir, normal, color, specular, intensity);
			}
		}
	}

	g_OutColor = vec4(min(c, vec3(1.0f)), 1.0f);
}