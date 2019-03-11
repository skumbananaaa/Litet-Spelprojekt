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

layout(std140, binding = 4) uniform PlaneBuffer
{
	vec4 g_ClipPlane;
};

layout(std140, binding = 5) uniform Extension
{
	float g_Extension;
};

out VS_OUT
{
	vec2 TexCoords;
	vec4 Color;
} vs_out;

void main()
{
	vec3 cameraRight = vec3(g_View[0][0], g_View[1][0], g_View[2][0]);
	vec3 cameraUp = vec3(g_View[0][1], g_View[1][1], g_View[2][1]);

	vs_out.TexCoords = g_TexCoord;
	vs_out.Color = g_InstanceColor;

	vec2 quadPosition = g_Position.xy * g_InstanceScale;
	vec3 position = g_InstancePosition + (cameraRight * quadPosition.x) + (cameraUp * quadPosition.y);

	position.x += g_Extension * floor(clamp(g_InstancePosition.y, 0.0f, 5.9f) / 2.0f);

	gl_ClipDistance[2] = dot(position, g_ClipPlane);

	gl_Position = g_ProjectionView * vec4(position, 1.0f);
}

#elif defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
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