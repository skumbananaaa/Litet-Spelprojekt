out vec4 FragColor;
in vec3 localPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(localPos)); // make sure to normalize localPos
    vec3 color = texture(equirectangularMap, uv).rgb;

	vec3 mapped = color / (color + vec3(1.0f));

    FragColor = vec4(pow(mapped, vec3(1.0f / 2.2f)), 1.0f);
}

// ripped from https://learnopengl.com/PBR/IBL/Diffuse-irradiance