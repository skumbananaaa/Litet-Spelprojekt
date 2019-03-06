#define LEVEL_SIZE LEVEL_SIZE_X * LEVEL_SIZE_Y * LEVEL_SIZE_Z

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

layout(std140, binding = 2) uniform DefferedMaterialBuffer
{
	vec4 g_Color;
	vec4 g_ClipPlane;
	float g_Specular;
	float g_HasDiffuseMap;
	float g_HasSpecularMap;
};

layout(std140, binding = 1) uniform LightBuffer
{
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
};

layout(std140, binding = 3) uniform WorldBuffer
{
	ivec4 g_Map[LEVEL_SIZE];
};

layout(std140, binding = 5) uniform Extension
{
	float g_Extension;
};

#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec2 g_TexCoords;
layout(location = 3) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 Normal;
	vec3 FragPosition;
	vec3 LightColor;
	vec3 PointLightColor;
	vec3 PointLightSpecular;
	vec3 Specular;
	vec2 TexCoords;
} vs_out;

vec3 CalcLightContribution(vec3 lightDir, vec3 lightCol, vec3 normal)
{
	return max(dot(normal, lightDir), 0.0f) * lightCol;
}

vec3 CalcSpecular(vec3 lightDir, vec3 lightCol, vec3 viewDir, vec3 normal)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), g_Specular);
	return lightCol * spec;
}

#define DIRECTIONAL_LIGHT_CONTRIBUTION 0.1f

void main()
{
	//Setup
	vec3 normal = normalize((g_InstanceModel * vec4(g_Normal, 0.0f)).xyz);
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0f);
	
	vs_out.Normal = normal;
	vs_out.FragPosition = worldPos.xyz;
	
	vec3 scale = vec3(length(g_InstanceModel[0].xyz), length(g_InstanceModel[1].xyz), length(g_InstanceModel[2].xyz));

	//Do extension
	worldPos.x += g_Extension * floor(clamp(g_InstanceModel[3].y, 0.0f, 5.9f) / 2.0f);

	//Viewdir
	vec3 viewDir = normalize(g_CameraPosition.xyz - worldPos.xyz);

	//CLIPPING WALLS
	vec3 toLookAt = normalize(g_CameraLookAt - (g_InstanceModel[3].xyz + vec3(g_Extension * floor(clamp(g_InstanceModel[3].y, 0.0f, 5.9f) / 2.0f), 0.0f, 0.0f)));
	vec3 cameraForward = normalize(g_CameraLookAt - g_CameraPosition);
	float dotToLookAtForward = dot(vec3(cameraForward.x, 0.0f, cameraForward.z), vec3(toLookAt.x, 0.0f, toLookAt.z));
	float cutWalls = 1.0f;

	if (dotToLookAtForward > 0.0f)
	{
		vec4 wallClipPlane = vec4(0.0f, -1.0f, 0.0f, g_InstanceModel[3].y - WALL_STUMP_FROM_CENTER);
		cutWalls = dot(worldPos, wallClipPlane);
	}

	//CLIPPING
	//gl_ClipDistance[0] = cutWalls;
	//gl_ClipDistance[1] = dot(worldPos, g_ClipDistances[1]); //DEPENDING ON LEVEL
	//gl_ClipDistance[2] = dot(worldPos, g_ClipDistances[2]); //DEPENDING ON LEVEL

	//Calculate light
	vec3 specular = vec3(0.0f);
	vec3 lightColor = vec3(0.0f);
	vec3 pointLightColor = vec3(0.0f);
	vec3 pointLightSpecular = vec3(0.0f);
	
	//Dirlights
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightCol = g_DirLights[i].Color.rgb;

		lightColor += DIRECTIONAL_LIGHT_CONTRIBUTION * CalcLightContribution(lightDir, lightCol, normal);
		specular += DIRECTIONAL_LIGHT_CONTRIBUTION * CalcSpecular(lightDir, lightCol, viewDir, normal);
	}

	//PointLights
	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		vec3 lightPos = vec3(g_PointLights[i].Position.xyz);
		lightPos.x += g_Extension * floor(clamp(lightPos.y, 0.0f, 5.9f) / 2.0f);

		vec3 lightDir = lightPos - worldPos.xyz;
		float dist = length(lightDir);

		float attenuation = 1.0f / sqrt(dist);
		vec3 lightCol = g_PointLights[i].Color.rgb * attenuation;
		lightDir = normalize(lightDir);

		pointLightColor += CalcLightContribution(lightDir, lightCol, normal);
		pointLightSpecular += CalcSpecular(lightDir, lightCol, viewDir, normal);
	}

	//Spotlights
	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		vec3 lightPos = vec3(g_SpotLights[i].Position.xyz);
		lightPos.x += g_Extension * floor(clamp(lightPos.y, 0.0f, 5.9f) / 2.0f);

		float light_attenuation = 1.0f;
		vec3 lightDir = lightPos - worldPos.xyz;
		vec3 targetDir = normalize(g_SpotLights[i].TargetDirection);
		float dist = length(lightDir);
		lightDir = lightDir / dist;
			
		float attenuation = 1.0f / ((dist));
		
		float theta = dot(lightDir, -targetDir);
		float epsilon = g_SpotLights[i].Angle - g_SpotLights[i].OuterAngle;
		float intensity = 10 * clamp((theta - g_SpotLights[i].OuterAngle) / epsilon, 0.0, 1.0);
	
		vec3 lightCol = g_SpotLights[i].Color.rgb * attenuation;
		if(theta > g_SpotLights[i].OuterAngle)
		{
			lightColor += CalcLightContribution(lightDir, lightCol, normal);
			specular += CalcSpecular(lightDir, lightCol, viewDir, normal);
		}
	}

	vs_out.PointLightColor = pointLightColor;
	vs_out.PointLightSpecular = pointLightSpecular;
	vs_out.Specular = specular;
	vs_out.LightColor = lightColor;

	vec2 texCoords = g_TexCoords;
	texCoords.x *= max(scale.x, scale.z) / 2.0f;
	vs_out.TexCoords = texCoords;

	gl_Position = g_ProjectionView * worldPos;
}


#elif defined(FRAGMENT_SHADER)
layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform samplerCube g_ShadowMap0;
layout(binding = 2) uniform samplerCube g_ShadowMap1;
layout(binding = 3) uniform samplerCube g_ShadowMap2;
layout(binding = 4) uniform samplerCube g_ShadowMap3;
layout(binding = 5) uniform samplerCube g_ShadowMap4;

in VS_OUT
{
	vec3 Normal;
	vec3 FragPosition;
	vec3 LightColor;
	vec3 PointLightColor;
	vec3 PointLightSpecular;
	vec3 Specular;
	vec2 TexCoords;
} fs_in;

layout(std140, binding = 8) uniform ShadowBuffer
{
	vec4 g_LightPos[NUM_POINT_LIGHTS];
};

float InvShadowCalc(vec3 fragPos, vec3 normal, samplerCube shadowMap, int lightIndex)
{
	vec3 toLight = fragPos - g_LightPos[lightIndex].xyz;
	float currentDepth = length(toLight);
	float closestDepth = texture(shadowMap, toLight).r * g_LightPos[lightIndex].w * 10.0f; //FARPLANE

	float bias = max(0.025f * (1.0f - dot(normal, normalize(toLight))), 0.0125f);
	return (currentDepth - bias) >= closestDepth ? 0.0f : 1.0f;
}

void main()
{
	vec3 normal = normalize(fs_in.Normal);

	//Color
	vec3 mappedColor = texture(g_DiffuseMap, fs_in.TexCoords).rgb * g_HasDiffuseMap;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasDiffuseMap);
	vec3 color = mappedColor + uniformColor;

	//Final lightcalculation
	vec3 ambient = color * vec3(0.2f);
	vec3 diffuse = color * fs_in.LightColor;
	vec3 specular = fs_in.Specular;
	vec3 lightColor = (diffuse + specular);

	vec3 startPointLightColor = ((color * fs_in.PointLightColor) + fs_in.PointLightSpecular);
	vec3 finalPointLightColor = vec3(0.0f);
	finalPointLightColor += InvShadowCalc(fs_in.FragPosition, normal, g_ShadowMap0, 0) * startPointLightColor;
	finalPointLightColor += InvShadowCalc(fs_in.FragPosition, normal, g_ShadowMap1, 1) * startPointLightColor;
	finalPointLightColor += InvShadowCalc(fs_in.FragPosition, normal, g_ShadowMap2, 2) * startPointLightColor;
	finalPointLightColor += InvShadowCalc(fs_in.FragPosition, normal, g_ShadowMap3, 3) * startPointLightColor;
	finalPointLightColor += InvShadowCalc(fs_in.FragPosition, normal, g_ShadowMap4, 4) * startPointLightColor;

	g_OutColor = vec4(min(ambient + lightColor + finalPointLightColor, vec3(1.0f)), 1.0f);
}
#endif