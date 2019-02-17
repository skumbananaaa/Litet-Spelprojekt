#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;

layout(std140, binding = 0) uniform PerObjectBlock
{
	mat4 g_Model;
};

layout(std140, binding = 1) uniform PerFrameBlock
{
	mat4 g_CameraCombined;
	vec4 g_CameraPosition;
};

out vec3 texCoords;

void main()
{
	gl_Position = g_CameraCombined * vec4(g_Position * 2.0, 1.0f);
	texCoords = g_Position * 2.0;
}

#elif defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;

in vec3 texCoords;

uniform samplerCube g_SkyBox;

void main()
{
	g_OutColor = texture(g_SkyBox, texCoords);
}
#endif