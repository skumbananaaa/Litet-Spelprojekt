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

layout(std140, binding = 2) uniform MaterialBuffer
{
	vec4 g_Color;
	vec4 g_ClipPlane;
	float g_Specular;
	float g_HasDiffuseMap;
};

layout(std140, binding = 4) uniform PlaneBuffer
{
	vec4 g_ReflectionClipPlane;
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

void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);
	vec3 normal = normalize((g_InstanceModel * vec4(g_Normal, 0.0)).xyz);

	worldPos.x += g_Extension * floor(clamp(g_InstanceModel[3].y, 0.0f, 5.9f) / 2.0f);

	gl_ClipDistance[1] = dot(worldPos, g_ReflectionClipPlane);
	gl_ClipDistance[2] = dot(worldPos, g_ClipPlane);

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
		lightPos.x += g_Extension * floor(clamp(lightPos.y, 0.0f, 5.9f) / 2.0f);

		vec3 lightDir = lightPos - worldPos.xyz;
		float dist = length(lightDir);

		float attenuation = 1.0f / sqrt(dist);
		vec3 lightCol = g_PointLights[i].Color.rgb * attenuation;
		lightDir = normalize(lightDir);

		lightColor += CalcLightContribution(lightDir, lightCol, normal);
		specular += CalcSpecular(lightDir, lightCol, viewDir, normal);
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
	//Color
	vec3 color = vec3(0.09f, 0.34f, 0.49f);

	//Final lightcalculation
	vec3 ambient = color * vec3(0.2f);
	vec3 diffuse = color * fs_in.LightColor;
	vec3 specular = fs_in.Specular;
	g_OutColor = vec4(min(ambient + diffuse + specular, vec3(1.0f)), 1.0f);
}
#endif