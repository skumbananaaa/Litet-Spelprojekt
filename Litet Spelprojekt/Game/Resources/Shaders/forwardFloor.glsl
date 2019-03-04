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

ivec3 CalcMapPos(vec3 worldpos)
{
	ivec3 mapPos = ivec3(round(worldpos.x), worldpos.y, round(worldpos.z));
	mapPos.x = clamp(mapPos.x, 0, 11);
	mapPos.y = clamp(mapPos.y, 0, 5);
	mapPos.z = clamp(mapPos.z, 0, 41);

	return mapPos;
}

uvec4 CalcRoomIndex(ivec3 mapPos)
{
	return uvec4(
		g_Map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].x,
		g_Map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].y,
		g_Map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].z,
		g_Map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].w
	);
}

void main()
{
	//Setup
	vec3 normal = normalize((g_InstanceModel * vec4(g_Normal, 0.0f)).xyz);
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);
	vec3 scale = vec3(length(g_InstanceModel[0].xyz), length(g_InstanceModel[1].xyz), length(g_InstanceModel[2].xyz));
	vs_out.Normal = normal;
	vs_out.FragPosition = worldPos.xyz;

	//Calculate position in tiles
	ivec3 mapPos = CalcMapPos(worldPos.xyz);
	uvec4 roomIndex = CalcRoomIndex(mapPos);

	//Do extension
	worldPos.x += g_Extension * floor(g_InstanceModel[3].y / 2.0f);

	//Viewdir
	vec3 viewDir = normalize(g_CameraPosition.xyz - worldPos.xyz);

	//Calculate light
	vec3 specular = vec3(0.0f);
	vec3 lightColor = vec3(0.0f);
	
	//Dirlights
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightCol = g_DirLights[i].Color.rgb;

		lightColor += CalcLightContribution(lightDir, lightCol, normal);
		specular += CalcSpecular(lightDir, lightCol, viewDir, normal);
	}

	//PointLights
	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		vec3 lightPos = vec3(g_PointLights[i].Position.xyz);
		lightPos.x += g_Extension * floor(lightPos.y / 2.0f);

		ivec3 lightMapPos = CalcMapPos(g_PointLights[i].Position.xyz);
		uvec4 lightRoomIndex = CalcRoomIndex(lightMapPos);

		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
		{
			vec3 lightDir = lightPos - worldPos.xyz;
			float dist = length(lightDir);

			float attenuation = 1.0f / (dist);
			vec3 lightCol = g_PointLights[i].Color.rgb * attenuation;
			lightDir = normalize(lightDir);

			lightColor += CalcLightContribution(lightDir, lightCol, normal);
			specular += CalcSpecular(lightDir, lightCol, viewDir, normal);
		}
	}

	//Spotlights
	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		vec3 lightPos = vec3(g_SpotLights[i].Position.xyz);
		lightPos.x += g_Extension * floor(lightPos.y / 2.0f);

		ivec3 lightMapPos = CalcMapPos(g_SpotLights[i].Position.xyz);
		uvec4 lightRoomIndex = CalcRoomIndex(lightMapPos);

		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
		{
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
	}

	vs_out.Specular = specular;
	vs_out.LightColor = lightColor;

	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ProjectionView * worldPos;
}


#elif defined(FRAGMENT_SHADER)
layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform samplerCube g_ShadowMap;

in VS_OUT
{
	vec3 Normal;
	vec3 FragPosition;
	vec3 LightColor;
	vec3 Specular;
	vec2 TexCoords;
} fs_in;

layout(std140, binding = 8) uniform ShadowBuffer
{
	vec3 g_LightPos;
	float g_FarPlane;
};

float ShadowCalc(vec3 fragPos, vec3 normal)
{
	vec3 toLight = fragPos - g_LightPos;
	float closestDepth = texture(g_ShadowMap, toLight).r * g_FarPlane;
	float currentDepth = length(toLight);

	float bias = 0.0f;// max(0.025f * (1.0f - dot(normal, toLight)), 0.0025f);
	return ((currentDepth - bias) > closestDepth) ? 1.0f : 0.0f;
}

void main()
{
	//Color
	vec3 mappedColor = texture(g_DiffuseMap, fs_in.TexCoords).rgb * g_HasDiffuseMap;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasDiffuseMap);
	vec3 color = mappedColor + uniformColor;

	//Final lightcalculation
	vec3 ambient = color * vec3(0.2f);
	vec3 diffuse = color * fs_in.LightColor;
	vec3 specular = fs_in.Specular;

	float shadow = ShadowCalc(fs_in.FragPosition, fs_in.Normal);
	g_OutColor = vec4(min(ambient + ((1.0f - shadow) * (diffuse + specular)), vec3(1.0f)), 1.0f);
}
#endif