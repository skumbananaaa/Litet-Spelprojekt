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

//DEFINES
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 3
#define NUM_SPOT_LIGHTS 2

#define LEVEL_SIZE 756

//DEFINE STRUCTS
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

//IN
layout(location = 0) in vec2 g_Position;
layout(location = 1) in vec4 g_Indicators;
layout(location = 2) in mat4 g_InstanceModel;

//UNIFORMS
layout(binding = 6) uniform sampler2D displacementMap;

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
	float g_WaveFactor;
};

//OUT
out VS_OUT
{
	vec4 ClipSpaceGrid;
	vec4 ClipSpaceReal;
	vec3 Normal;
	vec3 ToCameraVector;
	vec3 Specular;
	vec3 Diffuse;
} vs_out;

//CONSTS
const float PI = 3.1415926535897932384626433832795f;

const float waveLength = 4.0f;
const float waveAmplitude = 0.2f;
const float specularReflectivity = 0.4f;
const float shineDamper = 20.0f;
const float specularStrength = 256.0f;//0.6;
const float yScalingFactor = 2.5f;

const vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
const vec3 lightBias = vec3(1.0f, 1.0f, 1.0f);

vec3 CalculateDiffuseLighting(vec3 toLightVector, vec3 normal, vec3 lightColor, float intensity)
{
	float brightness = max(dot(toLightVector, normal), 0.0f);
	return lightColor * brightness * intensity;
}

vec3 CalcSpecularLighting(vec3 toCamVector, vec3 toLightVector, vec3 normal, vec3 lightColor, float intensity, float specularIntensity)
{
	vec3 halfwayDir = normalize(toLightVector + toCamVector);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specularIntensity);
	return spec * lightColor * intensity;
}

vec3 calcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2)
{
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

float generateOffset(float x, float z, float val1, float val2)
{
	float radiansX = ((mod(x + z * x * val1, waveLength) / waveLength) + g_WaveFactor * mod(x * 0.8f + z, 1.5f)) * 2.0f * PI;
	float radiansZ = ((mod(val2 * (z * x + x * z), waveLength) / waveLength) + g_WaveFactor * 2.0f * mod(x , 2.0f)) * 2.0f * PI;
	return waveAmplitude * 0.5f * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex)
{
	float xDistortion = generateOffset(vertex.x, vertex.z, 0.2f, 0.1f);
	float yDistortion = yScalingFactor * generateOffset(vertex.x, vertex.z, 0.1f, 0.3f);
	float zDistortion = generateOffset(vertex.x, vertex.z, 0.15f, 0.2f);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

void main()
{
	//vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);

	//get the grid position of all 3 vertices in the triangle
	vec3 currentVertex = vec3(g_Position.x, 0.0f, g_Position.y);
	vec3 vertex1 = currentVertex + vec3(g_Indicators.x, 0.0f, g_Indicators.y);
	vec3 vertex2 = currentVertex + vec3(g_Indicators.z, 0.0f, g_Indicators.w);
	
	vs_out.ClipSpaceGrid = g_ProjectionView * vec4(currentVertex, 1.0f);
	
	//apply distortion to all 3 vertices
	currentVertex = applyDistortion(currentVertex);
	vertex1 = applyDistortion(vertex1);
	vertex2 = applyDistortion(vertex2);
	
	vs_out.Normal = calcNormal(currentVertex, vertex1, vertex2);
	vs_out.ClipSpaceReal = g_ProjectionView * vec4(currentVertex, 1.0f);
	vs_out.ToCameraVector = normalize(g_CameraPosition - currentVertex);
	
	ivec3 mapPos = ivec3(round(currentVertex.x), currentVertex.y, round(currentVertex.z));
	mapPos.x = clamp(mapPos.x, 0, 11);
	mapPos.y = clamp(mapPos.y, 0, 5);
	mapPos.z = clamp(mapPos.z, 0, 41);

	uint roomIndex[] = {
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].x,
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].y,
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].z,
		map[int(mapPos.x * 63 + mapPos.y * 10.5 + mapPos.z * 0.25)].w
	};

	vs_out.Diffuse = vec3(0.0f);
	vs_out.Specular = vec3(0.0f);

	//Calculate lighting
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;

		vs_out.Diffuse += CalculateDiffuseLighting(lightDir, vs_out.Normal, lightColor, 1.0f);
		vs_out.Specular += CalcSpecularLighting(vs_out.ToCameraVector, lightDir, vs_out.Normal, lightColor, 1.0f, specularStrength);
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
			vec3 lightDir = g_PointLights[i].Position.xyz - currentVertex;
			float dist = length(lightDir);

			float attenuation = 1.0f / (dist * dist);
			vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
			lightDir = normalize(lightDir);
			float cosTheta = dot(vs_out.Normal, lightDir);

			vs_out.Diffuse += CalculateDiffuseLighting(lightDir, vs_out.Normal, lightColor, 1.0f);
			vs_out.Specular += CalcSpecularLighting(vs_out.ToCameraVector, lightDir, vs_out.Normal, lightColor, 1.0f, specularStrength);
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
			vec3 lightDir = g_SpotLights[i].Position.xyz - currentVertex;
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
				vs_out.Diffuse += CalculateDiffuseLighting(lightDir, vs_out.Normal, lightColor, intensity);
				vs_out.Specular += CalcSpecularLighting(vs_out.ToCameraVector, lightDir, vs_out.Normal, lightColor, intensity, specularStrength);
			}
		}
	}

	gl_Position = vs_out.ClipSpaceReal;
}

#elif defined(FRAGMENT_SHADER)

layout(early_fragment_tests) in;

//IN
in VS_OUT
{
	in vec4 ClipSpaceGrid;
	in vec4 ClipSpaceReal;
	in vec3 Normal;
	in vec3 ToCameraVector;
	in vec3 Specular;
	in vec3 Diffuse;
} fs_in;

//UNIFORMS
layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D dudvMap;
layout(binding = 4) uniform sampler2D reflectionTexture;

//OUT
layout(location = 0) out vec4 g_OutColor;

//CONSTS
const float distortionStrength = 0.005;
const float specularStrength = 256.0f;//0.6;
const float shininess = 20.0;
const float refractionExp = 1.5;
const float normalYSmoothness = 4.0;
const float depthOfFullOpaque = 0.7;
const float depthOfFullDistortion = 0.9;
const float depthOfFullSpecular = 0.7;

const vec3 waterColour = vec3(0.604, 0.867, 0.851);
const float fresnelReflective = 0.5;
const float edgeSoftness = 1;
const float minBlueness = 0.4;
const float maxBlueness = 0.8;
const float murkyDepth = 14;

const float fogDensity = 0.075;
const float fogGradient = 5.0;

const vec2 nearFarPlanes = vec2(0.1f, 100.0f);

//FUNCTIONS
float toLinearDepth(float zDepth)
{
	float near = nearFarPlanes.x;
	float far = nearFarPlanes.y;
	return 2.0 * near * far / (far + near - (2.0 * zDepth - 1.0) * (far - near));
}

float calculateFresnel()
{
	vec3 viewVector = normalize(fs_in.ToCameraVector);
	vec3 normal = normalize(fs_in.Normal);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, fresnelReflective);
	return clamp(refractiveFactor, 0.0, 1.0);
}

vec2 clipSpaceToTexCoords(vec4 clipSpace)
{
	vec2 ndc = (clipSpace.xy / clipSpace.w);
	vec2 texCoords = ndc / 2.0 + 0.5;
	return clamp(texCoords, 0.002, 0.998);
}

void main()
{
	vec2 texCoordsReal = clipSpaceToTexCoords(fs_in.ClipSpaceReal);
	vec2 texCoordsGrid = clipSpaceToTexCoords(fs_in.ClipSpaceGrid);
	
	vec2 refractionTexCoords = texCoordsGrid;
	vec2 reflectionTexCoords = vec2(texCoordsGrid.x, 1.0f - texCoordsGrid.y);
	
	vec3 refractColour = vec3(0.08f, 0.07f, 0.39f);
	vec3 reflectColour = texture(reflectionTexture, reflectionTexCoords).rgb;
	
	//apply some blueness
	reflectColour = mix(reflectColour, waterColour, minBlueness);
	
	vec3 finalColour = mix(reflectColour, refractColour, calculateFresnel());
	finalColour = finalColour * fs_in.Diffuse + fs_in.Specular;
	
	g_OutColor = vec4(finalColour, 1.0f);
}
#endif