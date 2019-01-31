#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2DMS g_Depth;

layout(std140, binding = 0) uniform PerFrame
{
	mat4 g_ViewProjection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
};

layout(std140, binding = 1) uniform PerObject
{
	mat4 g_Model;
	mat4 g_InverseModel;
	vec4 g_Direction;
};

in VS_OUT
{
	vec4 PositionClipSpace;
} fs_in;

vec3 PositionFromDepth(vec2 projCoord, float depth)
{
	float z = (depth * 2.0f) - 1.0f;

	vec4 posViewSpace = g_InverseProjection * vec4(projCoord, z, 1.0f);
	posViewSpace = posViewSpace / posViewSpace.w;

	vec4 worldPosition = g_InverseView * vec4(posViewSpace.xyz, 1.0f);
	return worldPosition.xyz;
}

float SampleDepth(vec2 texCoords)
{
	ivec2 size = textureSize(g_Depth);
	ivec2 coords = ivec2(size * texCoords);

	float depth = 1.0f;
	for (int i = 0; i < 2; i++)
	{
		depth = min(depth, texelFetch(g_Depth, coords, i).r);
	}

	return depth;
}

void main()
{
	vec2 projCoord = fs_in.PositionClipSpace.xy / fs_in.PositionClipSpace.w;
	vec2 texCoords = (projCoord * 0.5f) + vec2(0.5f);
	float sampledDepth = SampleDepth(texCoords);

	vec3 worldPosition = PositionFromDepth(projCoord, sampledDepth);
	vec3 objectPosition = (g_InverseModel * vec4(worldPosition, 1.0f)).xyz;

	//Reject pixels outside
	if (0.5f - abs(objectPosition.x) < 0.0f || 0.5f - abs(objectPosition.y) < 0.0f || 0.5f - abs(objectPosition.z) < 0.0f)
	{
		discard;
	}

	vec2 decalTexCoords = objectPosition.xz + vec2(0.5f);
	
	//COLOR
	vec4 mappedTexture = texture(g_Texture, decalTexCoords);
	g_OutColor = mappedTexture;

	//NORMAL
	vec3 ddxWp = dFdx(worldPosition);
	vec3 ddyWp = dFdy(worldPosition);

	vec3 binormal = normalize(ddxWp);
	vec3 tangent = normalize(ddyWp);
	vec3 normal = cross(binormal, tangent);

	//Reject if on a side
	if (max(dot(normal, normalize(g_Direction.xyz)), 0.0f) < 0.2f)
	{
		discard;
	}
	
	vec3 mappedNormal = (texture(g_NormalMap, decalTexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(tangent, binormal, normal);
	mappedNormal = tbn * mappedNormal;

	mappedNormal = (normalize(mappedNormal) + vec3(1.0f)) * 0.5f;
	g_Normal = vec4(normalize(mappedNormal), mappedTexture.a);
}