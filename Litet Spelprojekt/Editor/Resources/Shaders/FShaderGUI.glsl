#version 420 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textureToSample;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 projection;
    vec3 color;
};

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textureToSample, TexCoords).r);
    FragColor = vec4(color, 1.0) * sampled;
}