#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec4 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;

layout(std140, binding = 0) uniform VSPerFrame
{
	mat4 g_ViewProjection;
	vec3 g_CameraPosition;
	float g_Padding;
	vec3 g_CameraLookAt;
};

layout(std140, binding = 1) uniform PerObject
{
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

void main()
{
	vec3 toLookAt = normalize(g_CameraLookAt.xyz - fs_in.Position.xyz);
	float dotLookAtNormal = dot(fs_in.Normal, toLookAt);

	if (dotLookAtNormal < 0.0f)
	{
		discard;
	}

	//COLOR
	vec3 mappedColor = texture(g_Texture, fs_in.TexCoords).rgb * g_HasTexture;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasTexture);
	g_OutColor = vec4(uniformColor + mappedColor, 1.0f);
	
	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	mappedNormal = tbn * mappedNormal;

	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);
	normal = (normalize(normal) + vec3(1.0f)) * 0.5f;

	g_Normal = vec4(normal, 1.0f);
}