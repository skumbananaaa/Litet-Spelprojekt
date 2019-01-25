#version 420 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 projection;
    vec3 color;
};

void main()
{
    gl_Position = projection * vec4(position, 0.0, 1.0);
    
	vec2 uv = texCoords;
	uv.y = 1.0f - uv.y;
	TexCoords = uv;
}  