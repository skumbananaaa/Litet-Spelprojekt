#version 420

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Color;
layout(binding = 1) uniform sampler2D g_NormalDepth;

layout(binding = 0) uniform LightPassBuffer
{
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec3 g_CameraPosition;
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

void main()
{
	vec4 normalDepth = texture(g_NormalDepth, fs_in.TexCoords);
	vec3 position = PositionFromDepth(normalDepth.w);

	vec3 normal = normalize(normalDepth.xyz);
	vec4 color = texture(g_Color, fs_in.TexCoords);
	vec3 lightDir = normalize(vec3(0.0f, 1.0f, 0.5f));
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 viewDir = normalize(g_CameraPosition - position);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//AMBIENT
	vec3 ambient = vec3(0.3f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f));

	//SPECULAR
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);
	vec3 specular = vec3(spec) * lightColor;

	vec3 finalColor = ((ambient + diffuse) * color.rgb * lightColor) + specular;
	g_OutColor = vec4(clamp(finalColor, 0.0f, 1.0f), 1.0f);
}