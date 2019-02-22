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
layout(location = 1) in vec2 g_TexCoord;
layout(location = 2) in vec3 g_InstancePosition;
layout(location = 3) in vec4 g_InstanceColor;
layout(location = 4) in vec2 g_InstanceScale;

layout(binding = 5) uniform Extension
{
	float extension;
};

out VS_OUT
{
	vec2 TexCoords;
	vec4 Color;
} vs_out;

void main()
{
	vs_out.TexCoords = g_TexCoord;
	vs_out.Color = g_InstanceColor;

	vec3 position = g_InstancePosition + (g_Position * g_InstanceScale.x);
	position.x += extension * floor(clamp(g_InstancePosition.y, 0.0f, 5.9f) / 2.0f);

	gl_Position = g_ProjectionView * vec4(position, 1.0f);
}

#elif defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
	vec4 Color;
} fs_in;

void main()
{
	g_OutColor = fs_in.Color;
}

#endif