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

out VS_OUT
{
	vec3 Normal;
	vec2 TexCoords;
	vec4 Color;
} vs_out;

void main()
{
	vec3 cameraRight = vec3(g_View[0][0], g_View[1][0], g_View[2][0]);
	vec3 cameraUp = vec3(g_View[0][1], g_View[1][1], g_View[2][1]);

	vs_out.Normal = g_CameraLookAt - g_CameraPosition;
	vs_out.TexCoords = g_TexCoord;
	vs_out.Color = g_InstanceColor;

	vec3 position = g_InstancePosition + (cameraRight * g_Position.x) + (cameraUp * g_Position.y);
	gl_Position = g_ProjectionView * vec4(position, 1.0f);
}

#elif defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec3 Normal;
	vec2 TexCoords;
	vec4 Color;
} fs_in;

layout(binding = 0) uniform sampler2D g_DiffuseMap;

void main()
{
	vec4 tex = texture(g_DiffuseMap, fs_in.TexCoords) * fs_in.Color;
	g_OutColor = tex;
}

#endif