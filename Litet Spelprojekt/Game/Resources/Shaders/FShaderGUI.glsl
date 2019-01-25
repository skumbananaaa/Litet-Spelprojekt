#version 420 core

in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D textureToSample;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 projection;
    vec3 color;
};

void main()
{    
    FragColor = texture(textureToSample, TexCoords);
}