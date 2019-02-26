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

#if defined(VERTEX_SHADER)
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_indicators;
layout(location = 2) in mat4 g_InstanceModel;

layout(binding = 6) uniform sampler2D displacementMap;

layout(std140, binding = 6) uniform WaterBuffer
{
	float g_DistortionFactor;
};

out VS_OUT
{
	vec3 Position;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} vs_out;

const float tiling = 4.0f;
const float displacement = 2.0f;
void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);

	vs_out.TexCoords = tiling * g_TexCoords;

	//ivec2 displacementMapSize = textureSize(displacementMap, 0);
	//ivec2 vertexTexCoords = ivec2(g_DistortionFactor) + ivec2(tiling * displacementMapSize * ((vs_out.TexCoords / 2.0f) + 0.5f));
	vec2 vertexTexCoords = vec2(g_DistortionFactor) + vs_out.TexCoords;
	worldPos.y += displacement * (textureLod(displacementMap, vertexTexCoords, 0).r - 0.5f) * 2.0f;

	vs_out.Position = worldPos.xyz;
	vs_out.ClipSpacePosition = g_ProjectionView * worldPos;

	gl_Position = vs_out.ClipSpacePosition;
}

#elif defined(FRAGMENT_SHADER)
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 3
#define NUM_SPOT_LIGHTS 2

#define LEVEL_SIZE 756

layout(early_fragment_tests) in;

const float distortionStrength = 0.005;
const float specularStrength = 256.0f;//0.6;
const float shininess = 20.0;
const float refractionExp = 1.5;
const float normalYSmoothness = 4.0;
const float depthOfFullOpaque = 0.7;
const float depthOfFullDistortion = 0.9;
const float depthOfFullSpecular = 0.7;

const float fogDensity = 0.075;
const float fogGradient = 5.0;

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

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec3 Position;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} fs_in;

layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D dudvMap;
layout(binding = 4) uniform sampler2D reflectionTexture;
layout(binding = 5) uniform sampler2D depthMap;

layout(std140, binding = 1) uniform LightBuffer
{
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
};

layout(std140, binding = 3) uniform WorldBuffer
{
	ivec4 map[LEVEL_SIZE];
};

layout(std140, binding = 6) uniform WaterBuffer
{
	float g_DistortionFactor;
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
	//vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
	//vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//vec3 refractionColor = vec3(0.09, 0.34, 0.49); //Ocean Blue
	vec3 refractionColor = vec3(0.08, 0.07, 0.39); //Dark Ocean Blue

	vec2 ndcTexCoords = (fs_in.ClipSpacePosition.xy / fs_in.ClipSpacePosition.w) / 2.0 + 0.5;

	vec2 reflectionTexCoords = vec2(ndcTexCoords.x, -ndcTexCoords.y);
	vec2 depthTexCoords = vec2(ndcTexCoords.x, ndcTexCoords.y);

	float near = 0.1;
	float far = 100.0; //FIX THESE AS UNIFORMS
	float depth = texture(depthMap, depthTexCoords).r;
	float groundDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = groundDistance - waterDistance;

	vec2 distortionTexCoords = texture(dudvMap, vec2(fs_in.TexCoords.x + g_DistortionFactor, fs_in.TexCoords.y)).rg * 0.1;
	distortionTexCoords = fs_in.TexCoords + vec2(distortionTexCoords.x, distortionTexCoords.y + g_DistortionFactor);
	vec2 totalDistortion = (texture(dudvMap, distortionTexCoords).rg * 2.0 - 1.0) * distortionStrength * clamp(waterDepth / depthOfFullDistortion, 0.0, 1.0);

	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999); //Fixar wobbly kant
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec3 reflectionColor = texture(reflectionTexture, reflectionTexCoords).rgb;

	//NORMAL
	vec3 normal = texture(normalMap, fs_in.TexCoords + vec2(g_DistortionFactor)).xyz;
	normal = normalize(vec3(normal.x * 2.0 - 1.0, normal.y * normalYSmoothness, normal.z * 2.0 - 1.0));

	vec3 viewDir = g_CameraPosition - fs_in.Position;
	float distFromCamera = length(viewDir);
	viewDir = normalize(viewDir);
	float refractionFactor = pow(max(dot(viewDir, normal), 0.0f), refractionExp);

	//Specular
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	//vec3 specular = specularStrength * spec * lightColor * clamp(waterDepth / depthOfFullSpecular, 0.0, 1.0);

	//Fog
	//float visibility = clamp(exp(-pow(distFromCamera * fogDensity, fogGradient)), 0.0, 1.0);

	vec4 col = vec4(mix(reflectionColor, refractionColor, refractionFactor), 1.0);
	col = mix(col, vec4(0.7, 0.25, 0.33, 1.0), 0.25);// + vec4(specular, 0.0);
	//FragColor = mix(vec4(0.392, 0.584, 0.929, 1.0), FragColor, visibility); //Fog
	//FragColor.a = clamp(waterDepth / depthOfFullOpaque, 0.0, 1.0);

	ivec3 mapPos = ivec3(round(fs_in.Position.x), fs_in.Position.y, round(fs_in.Position.z));
	mapPos.x = clamp(mapPos.x, 0, 11);
	mapPos.y = clamp(mapPos.y, 0, 5);
	mapPos.z = clamp(mapPos.z, 0, 41);

	uint roomIndex[] = {
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].x,
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].y,
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].z,
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].w
	};

	//Do lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, col.rgb, specularStrength, 1.0f);
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		ivec3 lightMapPos = ivec3(round(g_PointLights[i].Position.x), g_PointLights[i].Position.y, round(g_PointLights[i].Position.z));
		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
		lightMapPos.z = clamp(lightMapPos.z, 0, 41);

		uint lightRoomIndex[] = {
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].x,
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].y,
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].z,
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].w
		};

		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
		{
			vec3 lightDir = g_PointLights[i].Position.xyz - fs_in.Position;
			float dist = length(lightDir);

			float attenuation = 1.0f / (dist * dist);
			vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
			lightDir = normalize(lightDir);
			float cosTheta = dot(normal, lightDir);

			c += CalcLight(lightDir, lightColor, viewDir, normal, col.rgb, specularStrength, 1.0f);
		}
	}

	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		ivec3 lightMapPos = ivec3(round(g_SpotLights[i].Position.x), g_SpotLights[i].Position.y, round(g_SpotLights[i].Position.z));
		lightMapPos.x = clamp(lightMapPos.x, 0, 11);
		lightMapPos.y = clamp(lightMapPos.y, 0, 5);
		lightMapPos.z = clamp(lightMapPos.z, 0, 41);

		uint lightRoomIndex[] = {
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].x,
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].y,
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].z,
			map[int(lightMapPos.x * 63 + lightMapPos.y * 10.5 + lightMapPos.z * 0.25)].w
		};
		
		if (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] != 1 && (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] || (lightRoomIndex[(lightMapPos.x * 252 + lightMapPos.y * 42 + lightMapPos.z) % 4] == 0 || roomIndex[(mapPos.x * 252 + mapPos.y * 42 + mapPos.z) % 4] == 0) && lightMapPos.y / 2 == mapPos.y / 2))
		{
			float light_attenuation = 1.0f;
			vec3 lightDir = g_SpotLights[i].Position.xyz - fs_in.Position;
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
				c += CalcLight(normalize(lightDir), lightColor, viewDir, normal, col.rgb, specularStrength, intensity);
			}
		}
	}

	g_OutColor = vec4(min(c, vec3(1.0f)), 1.0f);
}
#endif