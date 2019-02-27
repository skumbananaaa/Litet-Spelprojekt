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
	vec3 LightColor;
	vec3 Specular;
	vec2 TexCoords;
} vs_out;

const float specularStrength = 256.0f;

vec3 CalcLightContribution(vec3 lightDir, vec3 lightCol, vec3 normal)
{
	return max(dot(normal, lightDir), 0.0f) * lightCol;
}

vec3 CalcSpecular(vec3 lightDir, vec3 lightCol, vec3 viewDir, vec3 normal)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specularStrength);
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
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);
	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0)).xyz;
	
	//Calculate position in tiles
	ivec3 mapPos = CalcMapPos(worldPos.xyz);
	uvec4 roomIndex = CalcRoomIndex(mapPos);

	worldPos.x += g_Extension * floor(g_InstanceModel[3].y / 2.0f);

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

layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D dudvMap;

layout(std140, binding = 6) uniform WaterBuffer
{
	float g_DistortionFactor;
};

in VS_OUT
{
	vec3 LightColor;
	vec3 Specular;
	vec2 TexCoords;
} fs_in;

void main()
{
	//SETUP
//	vec4 col = vec4(0.09f, 0.34f, 0.49f, 1.0f); //Ocean Blue
//	vec3 inNormal = normalize(fs_in.Normal);
//	vec2 ndcTexCoords = (fs_in.ClipSpacePosition.xy / fs_in.ClipSpacePosition.w) / 2.0 + 0.5;
//
//	vec2 distortionTexCoords = texture(dudvMap, vec2(fs_in.TexCoords.x + g_DistortionFactor, fs_in.TexCoords.y)).rg * 0.1f;
//	distortionTexCoords = fs_in.TexCoords + vec2(distortionTexCoords.x, distortionTexCoords.y + g_DistortionFactor);
//
//	vec3 worldPos = vec3(fs_in.Position);
//	worldPos.x += g_Extension * floor(worldPos.y / 2.0f);
//
//	//VIEWDIR
//	vec3 viewDir = g_CameraPosition - worldPos;
//	float distFromCamera = length(viewDir);
//	viewDir = normalize(viewDir);
//
//	ivec3 mapPos = ivec3(round(fs_in.Position.x), fs_in.Position.y, round(fs_in.Position.z));
//	mapPos.x = clamp(mapPos.x, 0, 11);
//	mapPos.y = clamp(mapPos.y, 0, 5);
//	mapPos.z = clamp(mapPos.z, 0, 41);
//
//	uint roomIndex[] = {
//		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].x,
//		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].y,
//		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].z,
//		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].w
//	};
//
//	//Do lightcalculation
//	vec3 c = vec3(0.0f);
//	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
//	{
//		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
//		vec3 lightColor = g_DirLights[i].Color.rgb;
//		float cosTheta = dot(inNormal, lightDir);
//
//		c += CalcLight(lightDir, lightColor, viewDir, inNormal, col.rgb, specularStrength, 1.0f);
//	}
//
//	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
//	{
//		ivec3 lightMapPos = ivec3(round(g_PointLights[i].Position.x), g_PointLights[i].Position.y, round(g_PointLights[i].Position.z));
//		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
//		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
//		lightMapPos.z = clamp(lightMapPos.z, 0, 41);
//
//		uint lightRoomIndex[] = {
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].x,
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].y,
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].z,
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].w
//		};
//
//		vec3 lightPos = vec3(g_PointLights[i].Position.xyz);
//		lightPos.x += g_Extension * floor(lightPos.y / 2.0f);
//
//		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
//		{
//			vec3 lightDir = lightPos - worldPos;
//			float dist = length(lightDir);
//
//			float attenuation = 1.0f / (dist * dist);
//			vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
//			lightDir = normalize(lightDir);
//			float cosTheta = dot(inNormal, lightDir);
//
//			c += CalcLight(lightDir, lightColor, viewDir, inNormal, col.rgb, specularStrength, 1.0f);
//		}
//	}
//
//	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
//	{
//		ivec3 lightMapPos = ivec3(round(g_SpotLights[i].Position.x), g_SpotLights[i].Position.y, round(g_SpotLights[i].Position.z));
//		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
//		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
//		lightMapPos.z = clamp(lightMapPos.z, 0, 41);
//
//		uint lightRoomIndex[] = {
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].x,
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].y,
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].z,
//			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].w
//		};
//		
//		vec3 lightPos = vec3(g_SpotLights[i].Position.xyz);
//		lightPos.x += g_Extension * floor(lightPos.y / 2.0f);
//
//		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
//		{
//			float light_attenuation = 1.0f;
//			vec3 lightDir = lightPos - worldPos;
//			vec3 targetDir = normalize(g_SpotLights[i].TargetDirection);
//			float dist = length(lightDir);
//			lightDir = normalize(lightDir);
//			
//			float attenuation = 1.0f / ((dist));
//			
//			vec3 lightColor = g_SpotLights[i].Color.rgb * attenuation;
//	
//			float theta = dot(lightDir, -targetDir);
//			float epsilon = g_SpotLights[i].Angle - g_SpotLights[i].OuterAngle;
//			float intensity = 10 * clamp((theta - g_SpotLights[i].OuterAngle) / epsilon, 0.0, 1.0);
//	
//			if(theta > g_SpotLights[i].OuterAngle)
//			{
//				c += CalcLight(normalize(lightDir), lightColor, viewDir, inNormal, col.rgb, specularStrength, intensity);
//			}
//		}
//	}

	//Color
	vec3 color = vec4(0.09f, 0.34f, 0.49f, 1.0f);

	//Final lightcalculation
	vec3 ambient = color * vec3(0.2f);
	vec3 diffuse = color * fs_in.LightColor;
	vec3 specular = fs_in.Specular;
	g_OutColor = vec4(min(ambient + diffuse + specular, vec3(1.0f)), 1.0f);
}
#endif