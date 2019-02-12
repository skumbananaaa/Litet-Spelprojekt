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
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;
layout(location = 8) in mat4 g_InstanceInverseModel;
layout(location = 12) in vec3 g_InstanceDirection;

out VS_OUT
{
	vec4 PositionClipSpace;
	mat4 InverseModel;
	vec3 Direction;
} vs_out;

void main()
{
	vs_out.Direction = g_InstanceDirection;
	vs_out.InverseModel = g_InstanceInverseModel;
	vs_out.PositionClipSpace = g_ProjectionView * g_InstanceModel * vec4(g_Position, 1.0f);

	gl_Position = vs_out.PositionClipSpace;
}


#elif defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 3) uniform sampler2DMS g_Depth;

layout(std140, binding = 2) uniform DefferedMaterialBuffer
{
	vec4 g_Color;
	vec4 g_ClipPlane;
	float g_Specular;
	float g_HasDiffuseMap;
	float g_HasNormalMap;
	float g_HasSpecularMap;
};

in VS_OUT
{
	vec4 PositionClipSpace;
	mat4 InverseModel;
	vec3 Direction;
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
	vec3 objectPosition = (fs_in.InverseModel * vec4(worldPosition, 1.0f)).xyz;

	//Reject pixels outside
	if (0.5f - abs(objectPosition.x) < 0.0f || 0.5f - abs(objectPosition.y) < 0.0f || 0.5f - abs(objectPosition.z) < 0.0f)
	{
		discard;
	}

	vec2 decalTexCoords = objectPosition.xz + vec2(0.5f);
	
	//COLOR
	vec4 mappedTexture = texture(g_Texture, decalTexCoords);
	mappedTexture.rgb = mappedTexture.rgb * g_HasDiffuseMap;
	g_OutColor = mappedTexture;

	//NORMAL
	vec3 ddxWp = dFdx(worldPosition);
	vec3 ddyWp = dFdy(worldPosition);

	vec3 binormal = normalize(ddxWp);
	vec3 tangent = normalize(ddyWp);
	vec3 normal = cross(binormal, tangent);

	//Reject if on a side
	if (max(dot(normal, normalize(fs_in.Direction)), 0.0f) < 0.2f)
	{
		discard;
	}
	
	vec3 mappedNormal = (texture(g_NormalMap, decalTexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(tangent, binormal, normal);
	mappedNormal = tbn * mappedNormal;
	mappedNormal = (normalize(mappedNormal) + vec3(1.0f)) * 0.5f;
	mappedNormal = (g_HasNormalMap * mappedNormal) + ((1.0 - g_HasNormalMap) * normal);

	g_Normal = vec4(normalize(mappedNormal), mappedTexture.a);
}
#endif