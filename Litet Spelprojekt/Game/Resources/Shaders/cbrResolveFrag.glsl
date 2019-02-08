#version 420

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 18
#define NUM_SPOT_LIGHTS 2

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
	vec4 Position;
	vec3 TargetDirection;
	float Angle;
	float OuterAngle;
	vec3 padding;
};

layout(binding = 0) uniform LightPassBuffer
{
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec4 g_CameraPosition;
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
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

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 viewDir, vec3 normal, vec3 color, float intensity, float cosTheta)
{
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	////AMBIENT
	//vec3 ambient = vec3(0.1f);
	////DIFFUSE
	//vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f)) * intensity;
	////SPECULAR
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);
	//vec3 specular = vec3(spec) * lightColor * intensity;
	//return ((ambient + diffuse) * color * lightColor) + specular;

	// ambient
	vec3 finalColor = vec3(0.1f) * color * lightColor;
	// diffuse
	if (cosTheta > 0)
	{
		finalColor += color * cosTheta * lightColor * intensity;
	}
	// specular
	vec3 eyeDir = viewDir;
	vec3 reflectRay = reflect(-lightDir, normal);
	float cosAlpha = clamp(dot(eyeDir, reflectRay), 0, 1);
	if (cosTheta > 0)
	{
		finalColor += lightColor * pow(cosAlpha, 256) * intensity;
	}
	clamp(finalColor, 0.0f, 1.0f);
	return finalColor;
}

vec4 sampleMSAATexture(sampler2DMS tex, vec2 nTexCoords)
{
	ivec2 texSize = textureSize(tex);
	ivec2 framebufSize = ivec2(texSize.x * 2, texSize.y);
	ivec2 texCoord = ivec2(nTexCoords * texSize);

	return texelFetch(tex, texCoord, framebufSize.x % 2);
}

void main()
{
	SpotLight spot1[2];
	
	spot1[0].Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	spot1[0].Position = vec4(6.0f, 6.0f, 10.0f, 0.0f);
	spot1[0].TargetDirection = vec3(0.0f, -1.0f, 0.0f);
	spot1[0].Angle = cos(radians(60.0f));
	spot1[0].OuterAngle = cos(radians(60.0f));

	spot1[1].Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	spot1[1].Position = vec4(6.0f, 6.0f, 20.0f, 0.0f);
	spot1[1].TargetDirection = vec3(0.0f, -1.0f, 0.0f);
	spot1[1].Angle = cos(radians(60.0f));
	spot1[1].OuterAngle = cos(radians(75.0f));

	float depth = sampleMSAATexture(g_Depth, fs_in.TexCoords).r;
	g_OutDepth = depth;

	vec3 normal = normalize(NormalDecode(sampleMSAATexture(g_Normal, fs_in.TexCoords).xyz));
	vec3 position = PositionFromDepth(depth);

	vec3 color = sampleMSAATexture(g_Color, fs_in.TexCoords).rgb;
	vec3 viewDir = normalize(g_CameraPosition.xyz - position);
	
	//Do lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, 1.0f, cosTheta);
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		vec3 lightDir = g_PointLights[i].Position.xyz - position;
		float dist = length(lightDir);

		float attenuation = 1.0f / (dist);
		vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
		lightDir = normalize(lightDir);
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, 1.0f, cosTheta);
	}

	// for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	// {
	// 	float light_attenuation = 1.0f;
	// 	vec3 lightDir = g_SpotLights[i].Position.xyz - position;
	// 	vec3 targetDir = normalize(g_SpotLights[i].TargetDirection);
	// 	float dist = length(lightDir);
	// 	lightDir = normalize(lightDir);
	// 	float cosTheta = dot(normal, lightDir);

	// 	float lightToSurfaceAngle = degrees(acos(dot(-lightDir, targetDir)));
	// 	float coneAngle = degrees(acos(g_SpotLights[i].Angle));
	// 	if (lightToSurfaceAngle > coneAngle)
	// 	{
	// 		light_attenuation += lightToSurfaceAngle - coneAngle;
	// 	}
	// 	float attenuation = 1.0f / (1.0 + light_attenuation * (dist));
		
	// 	vec3 lightColor = g_SpotLights[i].Color.rgb * attenuation;

	// 	float theta = dot(lightDir, -targetDir);
	// 	float epsilon = g_SpotLights[i].Angle - g_SpotLights[i].OuterAngle;
	// 	float intensity = clamp((theta - g_SpotLights[i].OuterAngle) / epsilon, 0.0, 1.0);

	// 	if(theta > g_SpotLights[i].OuterAngle)
	// 	{
	// 		c += CalcLight(lightDir, lightColor, viewDir, normal, color, intensity, cosTheta);
	// 	}
	// 	if (i == 1)
	// 	{
	// 		//c = vec3(g_SpotLights[0].Angle, g_SpotLights[0].OuterAngle, 0.0f);
	// 		c = vec3(attenuation, attenuation, attenuation);
	// 		break;
	// 	}
	// }

	for (int i = 0; i < 2; i++)
	{
		float light_attenuation = 1.0f;
		vec3 lightDir = spot1[i].Position.xyz - position;
		vec3 targetDir = normalize(spot1[i].TargetDirection);
		float dist = length(lightDir);
		lightDir = normalize(lightDir);
		float cosTheta = dot(normal, lightDir);

		float lightToSurfaceAngle = degrees(acos(dot(-lightDir, targetDir)));
		float coneAngle = degrees(acos(spot1[i].Angle));
		if (lightToSurfaceAngle > coneAngle)
		{
			light_attenuation += lightToSurfaceAngle - coneAngle;
		}
		float attenuation = 1.0f / (1.0 + light_attenuation * (dist));
		
		vec3 lightColor = spot1[i].Color.rgb * attenuation;

		float theta = dot(lightDir, -targetDir);
		float epsilon = spot1[i].Angle - spot1[i].OuterAngle;
		float intensity = clamp((theta - spot1[i].OuterAngle) / epsilon, 0.0, 1.0);

		if(theta > spot1[i].OuterAngle)
		{
			c += CalcLight(lightDir, lightColor, viewDir, normal, color, intensity, cosTheta);
		}
		// if (i == 1)
		// {
		// 	//c = vec3(g_SpotLights[0].Angle, g_SpotLights[0].OuterAngle, 0.0f);
		// 	c = vec3(attenuation, attenuation, attenuation);
		// 	break;
		// }
	}

	g_OutColor = vec4(min(c, vec3(1.0f)), 1.0f);
}