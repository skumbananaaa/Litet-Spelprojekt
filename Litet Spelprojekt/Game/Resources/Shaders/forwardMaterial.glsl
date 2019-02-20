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

layout(std140, binding = 2) uniform MaterialBuffer
{
	vec4 g_Color;
	vec4 g_ClipPlane;
	float g_Specular;
	float g_HasDiffuseMap;
	float g_HasNormalMap;
	float g_HasSpecularMap;
};

#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

layout(std140, binding = 4) uniform PlaneBuffer
{
	vec4 g_ReflectionClipPlane;
};

out VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} vs_out;

void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0f);

	gl_ClipDistance[0] = dot(worldPos, g_ClipPlane);
	gl_ClipDistance[1] = dot(worldPos, g_ReflectionClipPlane);

	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (g_InstanceModel * vec4(g_Tangent, 0.0f)).xyz;

	vs_out.WorldPosition = worldPos.xyz;
	vs_out.ObjectPosition = g_InstanceModel[3].xyz;
	vs_out.Normal = normal;
	vs_out.Tangent = tangent;
	vs_out.Binormal = cross(vs_out.Normal, vs_out.Tangent);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ProjectionView * worldPos;
}


#elif defined(FRAGMENT_SHADER)
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 3
#define NUM_SPOT_LIGHTS 2

#define LEVEL_SIZE 756

layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_SpecularMap;

in VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

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

layout(binding = 1) uniform LightBuffer
{
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
};

layout(binding = 3) uniform WorldBuffer
{
	ivec4 map[LEVEL_SIZE];
	int concealed;
	int extended;
};

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 viewDir, vec3 normal, vec3 color, float specularIntensity, float intensity)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//AMBIENT
	vec3 ambient = vec3(0.2f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f)) * intensity;

	//SPECULAR
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specularIntensity);
	vec3 specular = vec3(spec) * lightColor * intensity;

	return ((ambient + diffuse) * color * lightColor) + specular;
}

void main()
{
	//VIEWDIR
	vec3 viewDir = normalize(g_CameraPosition.xyz - fs_in.WorldPosition);

	//SPECULAR
	float specular = (texture(g_SpecularMap, fs_in.TexCoords).r * g_HasSpecularMap) + ((g_Specular) * (1.0f - g_HasSpecularMap));

	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	mappedNormal = tbn * mappedNormal;
	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);

	//COLOR
	vec3 mappedColor = texture(g_DiffuseMap, fs_in.TexCoords).rgb * g_HasDiffuseMap;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasDiffuseMap);
	vec3 color = mappedColor + uniformColor;

	ivec3 mapPos = ivec3(round(fs_in.WorldPosition.x) - floor(fs_in.WorldPosition.y / 2.0f) * 10 * extended, fs_in.WorldPosition.y, round(fs_in.WorldPosition.z) + 1);
	mapPos.x = clamp(mapPos.x, 0, 11);
	mapPos.y = clamp(mapPos.y, 0, 5);
	mapPos.z = clamp(mapPos.z, 0, 41);

	uint roomIndex[] = {
		map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].x,
		map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].y,
		map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].z,
		map[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) / 4].w
	};

	//Do lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, specular, 1.0f);
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		ivec3 lightMapPos = ivec3(round(g_PointLights[i].Position.x) - floor(g_PointLights[i].Position.y / 2.0f) * 10 * extended, g_PointLights[i].Position.y, round(g_PointLights[i].Position.z));
		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
		lightMapPos.z = clamp(lightMapPos.z, 0, 41);

		uint lightRoomIndex[] = {
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].x,
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].y,
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].z,
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].w
		};

		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
		{
			vec3 lightDir = g_PointLights[i].Position.xyz - fs_in.WorldPosition;
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
		ivec3 lightMapPos = ivec3(round(g_SpotLights[i].Position.x) - floor(g_PointLights[i].Position.y / 2.0f) * 10 * extended, g_SpotLights[i].Position.y, round(g_SpotLights[i].Position.z));
		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
		lightMapPos.z = clamp(lightMapPos.z, 0, 41);

		uint lightRoomIndex[] = {
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].x,
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].y,
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].z,
			map[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) / 4].w
		};
		
		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
		{
			float light_attenuation = 1.0f;
			vec3 lightDir = g_SpotLights[i].Position.xyz - fs_in.WorldPosition;
			vec3 targetDir = normalize(g_SpotLights[i].TargetDirection);
			float dist = length(lightDir);
			lightDir = normalize(lightDir);
			
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

	g_OutColor = vec4(c, 1.0f);
}
#endif