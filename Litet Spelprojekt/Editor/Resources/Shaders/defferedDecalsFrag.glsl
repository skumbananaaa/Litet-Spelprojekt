#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_Depth;

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
};

in VS_OUT
{
	vec4 PositionClipSpace;
} fs_in;

vec3 PositionFromDepth(vec2 projCoord, float depth)
{
	float z = (depth * 2.0f) - 1.0f;

	vec4 posClipSpace = vec4(projCoord, z, 1.0f);
	vec4 posViewSpace = g_InverseProjection * posClipSpace;

	posViewSpace = posViewSpace / posViewSpace.w;
	vec4 worldPosition = g_InverseView * posViewSpace;
	return worldPosition.xyz;
}

void main()
{
	vec2 projCoord = fs_in.PositionClipSpace.xy / fs_in.PositionClipSpace.w;
	vec2 texCoords = (projCoord * 0.5f) + vec2(0.5f);
	float sampledDepth = texture(g_Depth, texCoords).r;

	vec3 worldPosition = PositionFromDepth(projCoord, sampledDepth);
	vec3 objectPosition = (g_InverseModel * vec4(worldPosition, 1.0f)).xyz;

	//Reject pixels outside
	if (0.5f - abs(objectPosition.x) < 0.0f || 0.5f - abs(objectPosition.y) < 0.0f || 0.5f - abs(objectPosition.z) < 0.0f)
		discard;

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

	//Reject if on a side TODO: Fix all sides
	if (abs(dot(normal, vec3(0.0f, 1.0f, 0.0f))) < 0.5f)
		discard;
	
	vec3 mappedNormal = (texture(g_NormalMap, decalTexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(tangent, binormal, normal);
	mappedNormal = tbn * mappedNormal;

	mappedNormal = (normalize(mappedNormal) + vec3(1.0f)) * 0.5f;
	g_Normal = vec4(normalize(mappedNormal), mappedTexture.a);
}