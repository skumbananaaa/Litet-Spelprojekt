layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 model;
};

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
	float padding;
	vec4 clipPlane;
};

out vec3 texCoords;

void main()
{
	vec4 worldPos = model * vec4(position, 1.0);
	gl_position = cameraCombined * worldPos;
	texCoords = position;
}