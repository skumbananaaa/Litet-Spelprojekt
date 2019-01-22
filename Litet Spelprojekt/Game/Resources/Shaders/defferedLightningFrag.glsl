#version 420

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Color;
layout(binding = 1) uniform sampler2D g_Normal;
layout(binding = 2) uniform sampler2D g_Position;

void main()
{
	vec4 color = texture(g_Color, fs_in.TexCoords);
	vec3 normal = normalize(texture(g_Normal, fs_in.TexCoords).xyz);
	vec3 position = texture(g_Position, fs_in.TexCoords).xyz;

	vec3 lightDir = normalize(vec3(0.0f, 1.0f, 0.5f));
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	//AMBIENT
	vec3 ambient = vec3(0.3f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f));

	vec3 finalColor = (ambient + diffuse) * color.rgb * lightColor;
	g_OutColor = vec4(clamp(finalColor, 0.0f, 1.0f), 1.0f);
}