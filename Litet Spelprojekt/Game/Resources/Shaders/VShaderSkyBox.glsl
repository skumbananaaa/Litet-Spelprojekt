
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 model;
};

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec4 cameraPosition;
};

out vec3 texCoords;

void main()
{
	//vec4 worldPos = model;
	gl_Position = cameraCombined * vec4(position * 2.0, 1.0f);
	texCoords = position * 2.0;
}