
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

layout (std140, binding = 0) uniform PerFrameBlock
{
	mat4 projection;
    vec3 color;
};

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  