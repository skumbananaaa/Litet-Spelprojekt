in VS_OUT
{
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
	float vafan;
	vec3 g_CameraFront;
};

layout(std140, binding = 1) uniform PerObject
{
	mat4 g_Model;
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

out vec4 g_OutColor;

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 normal, vec3 color, float intensity)
{
	//AMBIENT
	vec3 ambient = vec3(0.1f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f)) * intensity;
	return ((ambient + diffuse) * color * lightColor);
}


void main()
{
	//COLOR
	vec3 mappedColor = texture(g_Texture, fs_in.TexCoords).rgb * g_HasTexture;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasTexture);
	vec3 c = uniformColor + mappedColor;

	vec3 lightDir = -normalize(g_CameraFront);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	g_OutColor = vec4(CalcLight(lightDir, lightColor, fs_in.Normal, c, 1.0f), 0.4);
}