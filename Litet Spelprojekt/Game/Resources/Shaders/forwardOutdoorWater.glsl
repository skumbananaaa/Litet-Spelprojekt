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
	vec2 g_WaveFactor;
};

//OUT
out VS_OUT
{
	vec4 ClipSpaceGrid;
	vec3 Normal;
	vec3 ToCameraVector;
	vec3 Specular;
	vec3 Diffuse;
} vs_out;

//CONSTS
const float specularStrength = 256.0f;//0.6;
const float waveScalingFactor = 0.75f;

vec3 mod289(vec3 x)
{
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) 
{
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) 
{
	return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
						0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
						-0.577350269189626,  // -1.0 + 2.0 * C.x
						0.024390243902439); // 1.0 / 41.0
	// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);

	// Other corners
	vec2 i1;
	//i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
	//i1.y = 1.0 - i1.x;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	// x0 = x0 - 0.0 + 0.0 * C.xx ;
	// x1 = x0 - i1 + 1.0 * C.xx ;
	// x2 = x0 - 1.0 + 2.0 * C.xx ;
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;

	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;

	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;

	// Normalise gradients implicitly by scaling m
	// Approximation of: m *= inversesqrt( a0*a0 + h*h );
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

	// Compute final noise value at P
	vec3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}

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

void main()
{
	//Get the grid position of all 3 vertices in the triangle
	vec3 currentVertex = vec3(g_Position.x, 0.0f, g_Position.y);
	vec3 vertex1 = currentVertex + vec3(g_Indicators.x, 0.0f, g_Indicators.y);
	vec3 vertex2 = currentVertex + vec3(g_Indicators.z, 0.0f, g_Indicators.w);
	
	vs_out.ClipSpaceGrid = g_ProjectionView * vec4(currentVertex, 1.0f);
	
	//Apply distortion to all 3 vertices
	currentVertex.y += waveScalingFactor * snoise(currentVertex.xz + g_WaveFactor);
	vertex1 += waveScalingFactor * snoise(vertex1.xz + g_WaveFactor);
	vertex2 += waveScalingFactor * snoise(vertex2.xz + g_WaveFactor);
	
	vs_out.Normal = calcNormal(currentVertex, vertex1, vertex2);
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

	gl_Position = g_ProjectionView * vec4(currentVertex, 1.0f);
}

#elif defined(FRAGMENT_SHADER)

layout(early_fragment_tests) in;

//IN
in VS_OUT
{
	in vec4 ClipSpaceGrid;
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
const vec3 waterColour = vec3(0.604, 0.867, 0.851);
const float fresnelReflective = 0.5;
const float minBlueness = 0.4;

const float fogDensity = 0.075;
const float fogGradient = 5.0;

//FUNCTIONS
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