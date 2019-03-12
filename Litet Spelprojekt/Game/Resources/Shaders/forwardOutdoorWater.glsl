#define LEVEL_SIZE LEVEL_SIZE_X * LEVEL_SIZE_Y * LEVEL_SIZE_Z

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

layout(std140, binding = 6) uniform WaterBuffer
{
	vec2 g_WaveFactor;
	vec2 g_IcebergPosition;
};

//OUT
out VS_OUT
{
	vec4 ClipSpaceGrid;
	vec3 Normal;
	vec3 ToCameraVector;
	vec3 LightColor;
	vec3 Specular;
	float FoamFactor;
} vs_out;

//CONSTS
const float specularStrength = 256.0f;//0.6;
const float waveScalingFactor = 0.3f;

vec3 mod289(vec3 x)
{
	return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec2 mod289(vec2 x) 
{
	return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec3 permute(vec3 x) 
{
	return mod289(((x * 34.0f) + 1.0f) * x);
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187f,
						0.366025403784439f,
						-0.577350269189626f,
						0.024390243902439f);
	// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v - i + dot(i, C.xx);

	// Other corners
	vec2 i1;
	i1 = (x0.x > x0.y) ? vec2(1.0f, 0.0f) : vec2(0.0f, 1.0f);
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;

	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute(permute(i.y + vec3(0.0f, i1.y, 1.0f))
		+ i.x + vec3(0.0f, i1.x, 1.0f));

	vec3 m = max(0.5f - vec3(dot(x0,x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0f);
	m = m*m ;
	m = m*m ;

	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

	vec3 x = 2.0f * fract(p * C.www) - 1.0f;
	vec3 h = abs(x) - 0.5f;
	vec3 ox = floor(x + 0.5f);
	vec3 a0 = x - ox;

	// Normalise gradients implicitly by scaling m
	// Approximation of: m *= inversesqrt( a0*a0 + h*h );
	m *= 1.79284291400159f - 0.85373472095314f * (a0 * a0 + h * h);

	// Compute final noise value at P
	vec3 g;
	g.x  = a0.x * x0.x  + h.x * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0f * dot(m, g);
}

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

vec3 CalcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2)
{
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

// Returns -1 if the x cooridnate should get lowered, otherwise returns 0
float GetDepthFromX(float x)
{
	return 1.0F - min(round((abs(x) - 1) / 5.0F), 1.0F);
}

// Returns -1 if the z cooridnate should get lowered, otherwise returns 0
float GetDepthFromZ(float z)
{
	return 1.0F - min(floor(abs(z) / 19.0F), 1.0F);
}

void main()
{
	//Get the grid position of all 3 vertices in the triangle
	vec3 currentVertex = vec3(g_Position.x, 1.3f, g_Position.y);
	vec3 vertex1 = currentVertex + vec3(g_Indicators.x, 0.0f, g_Indicators.y);
	vec3 vertex2 = currentVertex + vec3(g_Indicators.z, 0.0f, g_Indicators.w);
	vs_out.FoamFactor = float(length(g_IcebergPosition - currentVertex.xz) < 8.0f);
	currentVertex.y += vs_out.FoamFactor * (10.0f * min(1.0f, 4.0f / length(g_IcebergPosition - currentVertex.xz)) + snoise(min(vec2(10.0f), roundEven(g_IcebergPosition - currentVertex.xz))));
	vertex1.y 		+= vs_out.FoamFactor * (10.0f * min(1.0f, 4.0f / length(g_IcebergPosition - vertex1.xz)) + snoise(min(vec2(10.0f), roundEven(g_IcebergPosition - vertex1.xz))));
	vertex2.y 		+= vs_out.FoamFactor * (10.0f * min(1.0f, 4.0f / length(g_IcebergPosition - vertex2.xz)) + snoise(min(vec2(10.0f), roundEven(g_IcebergPosition - vertex2.xz))));
	
	float depth = 30.0F;
	float xOffset = 5.0F;
	float zOffset = 20.0F;

	float moveDown1 = GetDepthFromX(currentVertex.x - xOffset) * GetDepthFromZ(currentVertex.z - zOffset);
	currentVertex.x += clamp(currentVertex.x - xOffset, -1.0F, 1.0F) * moveDown1 * 2;
	currentVertex.z += clamp(currentVertex.z - zOffset, -1.0F, 1.0F) * moveDown1 * 2;
	currentVertex.y -= moveDown1 * depth;

	float moveDown2 = GetDepthFromX(vertex1.x - xOffset) * GetDepthFromZ(vertex1.z - zOffset);
	vertex1.x += clamp(vertex1.x - xOffset, -1.0F, 1.0F) * moveDown2 * 2;
	vertex1.z += clamp(vertex1.z - zOffset, -1.0F, 1.0F) * moveDown2 * 2;
	vertex1.y -= moveDown2 * depth;

	float moveDown3 = GetDepthFromX(vertex2.x - xOffset) * GetDepthFromZ(vertex2.z - zOffset);
	vertex2.x += clamp(vertex2.x - xOffset, -1.0F, 1.0F) * moveDown3 * 2;
	vertex2.z += clamp(vertex2.z - zOffset, -1.0F, 1.0F) * moveDown3 * 2;
	vertex2.y -= moveDown3 * depth;


	vs_out.ClipSpaceGrid = g_ProjectionView * vec4(currentVertex, 1.0f);
	
	//Apply distortion to all 3 vertices
	float oneMinusFoamFactor = 1.0f - vs_out.FoamFactor;
	currentVertex.y += waveScalingFactor * snoise(currentVertex.xz + g_WaveFactor) * oneMinusFoamFactor;
	vertex1 += waveScalingFactor * snoise(vertex1.xz + g_WaveFactor) * oneMinusFoamFactor;
	vertex2 += waveScalingFactor * snoise(vertex2.xz + g_WaveFactor) * oneMinusFoamFactor;
	
	vec3 normal = CalcNormal(currentVertex, vertex1, vertex2);
	vs_out.Normal = normal;
	vs_out.ToCameraVector = normalize(g_CameraPosition - currentVertex);

	//Viewdir
	vec3 viewDir = normalize(g_CameraPosition.xyz - currentVertex.xyz);

	//Calculate light
	vec3 specular = vec3(0.0f);
	vec3 lightColor = vec3(0.0f);

	//Dirlights
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightCol = g_DirLights[i].Color.rgb;

		lightColor += 3.0f * CalcLightContribution(lightDir, lightCol, normal);
		specular += CalcSpecular(lightDir, lightCol, viewDir, normal);
	}

	vs_out.Specular = specular;
	vs_out.LightColor = lightColor;

	gl_Position = g_ProjectionView * vec4(currentVertex, 1.0f);
}

#elif defined(FRAGMENT_SHADER)

layout(early_fragment_tests) in;

//IN
in VS_OUT
{
	vec4 ClipSpaceGrid;
	vec3 Normal;
	vec3 ToCameraVector;
	vec3 LightColor;
	vec3 Specular;
	float FoamFactor;
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
float CalculateFresnel()
{
	vec3 viewVector = normalize(fs_in.ToCameraVector);
	vec3 normal = normalize(fs_in.Normal);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, fresnelReflective);
	return clamp(refractiveFactor, 0.0, 1.0);
}

vec2 ClipSpaceToTexCoords(vec4 clipSpace)
{
	vec2 ndc = (clipSpace.xy / clipSpace.w);
	vec2 texCoords = ndc / 2.0 + 0.5;
	return clamp(texCoords, 0.002, 0.998);
}

void main()
{
	vec2 texCoordsGrid = ClipSpaceToTexCoords(fs_in.ClipSpaceGrid);
	
	vec2 refractionTexCoords = texCoordsGrid;
	vec2 reflectionTexCoords = vec2(texCoordsGrid.x, 1.0f - texCoordsGrid.y);
	
	vec3 refractColour = vec3(0.08f, 0.07f, 0.39f);
	vec3 reflectColour = texture(reflectionTexture, reflectionTexCoords).rgb;
	
	//apply some blueness
	reflectColour = mix(reflectColour, waterColour, minBlueness);
	
	vec3 finalColour = mix(reflectColour, refractColour, CalculateFresnel());
	finalColour = mix(finalColour, vec3(1.0F, 1.0F, 1.0F), fs_in.FoamFactor);
	finalColour = finalColour * fs_in.LightColor + fs_in.Specular;
	
	g_OutColor = vec4(finalColour, 1.0f);
}
#endif