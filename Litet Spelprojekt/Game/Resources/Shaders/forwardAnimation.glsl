#define MAX_BONES 50

vec3 EncodeNormals(vec3 normal)
{
	return (normalize(normal) + vec3(1.0f)) * 0.5f;
}

float EncodeSpecular(float specular)
{
	return specular / 256.0f;
}

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

layout(std140, binding = 7) uniform BoneBuffer
{
	mat4 g_Bones[MAX_BONES];
	mat4 g_Model;
};


#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in ivec4 g_BonesIDs;
layout(location = 5) in vec4 g_Weights;

out VS_OUT
{
	vec3 WorldPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} vs_out;

void main()
{
	mat4 BoneTransform = g_Bones[g_BonesIDs[0]] * g_Weights[0];
	BoneTransform += g_Bones[g_BonesIDs[1]] * g_Weights[1];
	BoneTransform += g_Bones[g_BonesIDs[2]] * g_Weights[2];
	BoneTransform += g_Bones[g_BonesIDs[3]] * g_Weights[3];

	mat4 finalModel = g_Model * BoneTransform;
	vec4 worldPos = finalModel * vec4(g_Position, 1.0);

	gl_ClipDistance[0] = dot(worldPos, g_ClipPlane);
	
	vec3 normal = (finalModel * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (finalModel * vec4(g_Tangent, 0.0f)).xyz;
	
	vs_out.WorldPosition = worldPos.xyz;
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

layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_SpecularMap;

in VS_OUT
{
	vec3 WorldPosition;
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
	//NORMALIZE
	vec3 inTangent = normalize(fs_in.Tangent);
	vec3 inBinormal = normalize(fs_in.Binormal);
	vec3 inNormal = normalize(fs_in.Normal);

	//VIEWDIR
	vec3 viewDir = normalize(g_CameraPosition.xyz - fs_in.WorldPosition);

	//SPECULAR
	float specular = (texture(g_SpecularMap, fs_in.TexCoords).r * g_HasSpecularMap) + ((g_Specular) * (1.0f - g_HasSpecularMap));

	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	mat3 tbn = mat3(inTangent, inBinormal, inNormal);
	mappedNormal = tbn * mappedNormal;
	vec3 normal = (inNormal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);

	//COLOR
	vec3 mappedColor = texture(g_DiffuseMap, fs_in.TexCoords).rgb * g_HasDiffuseMap;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasDiffuseMap);
	vec3 color = mappedColor + uniformColor;

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
		vec3 lightDir = g_PointLights[i].Position.xyz - fs_in.WorldPosition;
		float dist = length(lightDir);

		float attenuation = 1.0f / (dist * dist);
		vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
		lightDir = normalize(lightDir);
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, specular, 1.0f);
	}

	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		float light_attenuation = 1.0f;
		vec3 lightDir = g_SpotLights[i].Position.xyz - fs_in.WorldPosition;
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

	g_OutColor = vec4(c, 1.0f);
}
#endif