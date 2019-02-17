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

#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} vs_out;

void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0f);
	gl_ClipDistance[0] = dot(worldPos, g_ClipPlane);

	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (g_InstanceModel * vec4(g_Tangent, 0.0f)).xyz;

	vs_out.WorldPosition = worldPos.xyz;
	vs_out.ObjectPosition = g_InstanceModel[3].xyz;
	vs_out.Normal = normal;
	vs_out.Tangent = tangent;
	vs_out.Binormal = cross(vs_out.Normal, vs_out.Tangent);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ProjectionView * worldPos;
}


#elif defined(FRAGMENT_SHADER)
layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_SpecularMap;

in VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

void main()
{
	//SPECULAR
	float specular = (texture(g_SpecularMap, fs_in.TexCoords).r * g_HasSpecularMap) + ((g_Specular) * (1.0f - g_HasSpecularMap));

	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);

	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	mappedNormal = tbn * mappedNormal;

	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);

	//COLOR
	vec3 mappedColor = texture(g_DiffuseMap, fs_in.TexCoords).rgb * g_HasDiffuseMap;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasDiffuseMap);
	g_OutColor = vec4(uniformColor + mappedColor, specular);
}
#endif