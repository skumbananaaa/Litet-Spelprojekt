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

layout(std140, binding = 2) uniform DefferedMaterialBuffer
{
	vec4 g_Color;
	vec4 g_ClipPlane;
	float g_Specular;
	float g_HasDiffuseMap;
	float g_HasNormalMap;
	float g_HasSpecularMap;
};

layout(std140, binding = 3) uniform DissolveBuffer
{
	vec4 g_ClipDistances[3];
	float g_DissolvePercentage;
};

#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} vs_out;

void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);

	//CLIPPING WALLS
	vec3 toLookAt = normalize(g_CameraLookAt - g_InstanceModel[3].xyz);
	vec3 cameraForward = normalize(g_CameraLookAt - g_CameraPosition);
	float dotToLookAtForward = dot(vec3(cameraForward.x, 0.0f, cameraForward.z), vec3(toLookAt.x, 0.0f, toLookAt.z));
	float cutWalls = 1.0f;

	if (dotToLookAtForward > 0.0f)
	{
		vec4 wallClipPlane = vec4(0.0f, -1.0f, 0.0f, g_InstanceModel[3].y - WALL_STUMP_FROM_CENTER);
		cutWalls = dot(worldPos, wallClipPlane);
	}

	gl_ClipDistance[0] = cutWalls;

	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (g_InstanceModel * vec4(g_Tangent, 0.0f)).xyz;

	//CLIPPING DEPENDING ON LEVEL
	gl_ClipDistance[1] = dot(worldPos, g_ClipDistances[1]);
	gl_ClipDistance[2] = dot(worldPos, g_ClipDistances[2]);

	vs_out.WorldPosition = worldPos.xyz;
	vs_out.ObjectPosition = g_InstanceModel[3].xyz;
	vs_out.Normal = normal;
	vs_out.Tangent = tangent;
	vs_out.Binormal = cross(vs_out.Normal, vs_out.Tangent);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ProjectionView * worldPos;
}


#elif defined(FRAGMENT_SHADER)
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 18
#define NUM_SPOT_LIGHTS 17

layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

layout(binding = 0) uniform sampler2D g_DiffuseMap;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_SpecularMap;

in VS_OUT
{
	vec3 WorldPosition;
	vec3 ObjectPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

vec3 mod289(vec3 x)
{
	return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec4 mod289(vec4 x)
{
	return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec4 permute(vec4 x)
{
	return mod289(((x * 34.0f) + 1.0f) * x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159f - 0.85373472095314f * r;
}

vec3 fade(vec3 t)
{
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float snoise(vec3 v)
{
	const vec2  C = vec2(1.0f / 6.0f, 1.0f / 3.0f);
	const vec4  D = vec4(0.0f, 0.5f, 1.0f, 2.0f);

	//First corner
	vec3 i = floor(v + dot(v, C.yyy));
	vec3 x0 = v - i + dot(i, C.xxx);

	//Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0f - g;
	vec3 i1 = min(g.xyz, l.zxy);
	vec3 i2 = max(g.xyz, l.zxy);

	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy;
	vec3 x3 = x0 - D.yyy;

	//Permutations
	i = mod289(i);
	vec4 p = permute(permute(permute(
		i.z + vec4(0.0f, i1.z, i2.z, 1.0f))
		+ i.y + vec4(0.0f, i1.y, i2.y, 1.0f))
		+ i.x + vec4(0.0f, i1.x, i2.x, 1.0f));

	//Gradients: 7x7 points over a square, mapped onto an octahedron.
	//The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857f; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;

	vec4 j = p - 49.0f * floor(p * ns.z * ns.z);  //  mod(p,7*7)

	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0f * x_);    // mod(j,N)

	vec4 x = x_ * ns.x + ns.yyyy;
	vec4 y = y_ * ns.x + ns.yyyy;
	vec4 h = 1.0f - abs(x) - abs(y);

	vec4 b0 = vec4(x.xy, y.xy);
	vec4 b1 = vec4(x.zw, y.zw);

	vec4 s0 = floor(b0) * 2.0f + 1.0f;
	vec4 s1 = floor(b1) * 2.0f + 1.0f;
	vec4 sh = -step(h, vec4(0.0f));

	vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy;
	vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww;

	vec3 p0 = vec3(a0.xy, h.x);
	vec3 p1 = vec3(a0.zw, h.y);
	vec3 p2 = vec3(a1.xy, h.z);
	vec3 p3 = vec3(a1.zw, h.w);

	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;

	//Mix final noise value
	vec4 m = max(0.6f - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0f);
	m = m * m;
	return 42.0f * dot(m*m, vec4(dot(p0, x0), dot(p1, x1), dot(p2, x2), dot(p3, x3)));
}

#define EPSILON 0.1f

struct DirectionalLight
{
	vec4 Color;
	vec4 Direction;
};

struct PointLight
{
	vec4 Color;
	vec4 Position;
};

struct SpotLight
{
	vec4 Color;
	vec3 Position;
	float Angle;
	vec3 TargetDirection;
	float OuterAngle;
};

layout(binding = 1) uniform LightBuffer
{
	DirectionalLight g_DirLights[NUM_DIRECTIONAL_LIGHTS];
	PointLight g_PointLights[NUM_POINT_LIGHTS];
	SpotLight g_SpotLights[NUM_SPOT_LIGHTS];
};

vec3 CalcLight(vec3 lightDir, vec3 lightColor, vec3 viewDir, vec3 normal, vec3 color, float specularIntensity, float intensity)
{
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//AMBIENT
	vec3 ambient = vec3(0.2f);

	//DIFFUSE
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0f)) * intensity;

	//SPECULAR
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specularIntensity);
	vec3 specular = vec3(spec) * lightColor * intensity;

	return ((ambient + diffuse) * color * lightColor) + specular;
}

void main()
{
	//VIEWDIR
	vec3 viewDir = normalize(g_CameraPosition.xyz - fs_in.WorldPosition);

	//SPECULAR
	float specular = (texture(g_SpecularMap, fs_in.TexCoords).r * g_HasSpecularMap) + (g_Specular * (1.0f - g_HasSpecularMap));

	//COLOR
	vec3 mappedColor = texture(g_DiffuseMap, fs_in.TexCoords).rgb * g_HasDiffuseMap;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasDiffuseMap);
	vec3 color = mappedColor + uniformColor;

	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);

	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	mappedNormal = tbn * mappedNormal;
	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);

	//DISSOLVE
	if (fs_in.ObjectPosition.y - fs_in.WorldPosition.y < WALL_STUMP_FROM_CENTER)
	{
		float minDissolve = g_DissolvePercentage * ((snoise(fs_in.WorldPosition.xyz) / 2.0f) + 0.5f);

		float isNotUp = abs(dot(fs_in.Normal, vec3(0.0f, 1.0f, 0.0f)));
		vec3 cameraForward = normalize(g_CameraLookAt.xyz - g_CameraPosition.xyz);
		vec3 objectToLookAt = vec3(g_CameraLookAt.x, 0.0f, g_CameraLookAt.z) - vec3(fs_in.ObjectPosition.x, 0.0f, fs_in.ObjectPosition.z);
		float distanceToLookAt = abs(dot(objectToLookAt, vec3(cameraForward.x, 0.0f, cameraForward.z)));

		if (distanceToLookAt < minDissolve)
		{
			if (distanceToLookAt < minDissolve - EPSILON)
			{
				discard;
			}

			float lerpValue = 0.5f + (((distanceToLookAt + EPSILON) - minDissolve) / EPSILON) / 2.0f;
			color = color * lerpValue;
		}
	}

	//Do lightcalculation
	vec3 c = vec3(0.0f);
	for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
	{
		vec3 lightDir = normalize(g_DirLights[i].Direction.xyz);
		vec3 lightColor = g_DirLights[i].Color.rgb;
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, specular, 1.0f);
	}

	for (uint i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		vec3 lightDir = g_PointLights[i].Position.xyz - fs_in.WorldPosition;
		float dist = length(lightDir);

		float attenuation = 1.0f / (dist * dist);
		vec3 lightColor = g_PointLights[i].Color.rgb * attenuation;
		lightDir = normalize(lightDir);
		float cosTheta = dot(normal, lightDir);

		c += CalcLight(lightDir, lightColor, viewDir, normal, color, specular, 1.0f);
	}

	for (uint i = 0; i < NUM_SPOT_LIGHTS; i++) 
	{
		float light_attenuation = 1.0f;
		vec3 lightDir = g_SpotLights[i].Position.xyz - fs_in.WorldPosition;
		vec3 targetDir = normalize(g_SpotLights[i].TargetDirection);
		float dist = length(lightDir);
		lightDir = normalize(lightDir);
		float cosTheta = dot(normal, lightDir);

		float lightToSurfaceAngle = degrees(acos(dot(-lightDir, targetDir)));
		float coneAngle = degrees(acos(g_SpotLights[i].Angle));
		if (lightToSurfaceAngle > coneAngle)
		{
			light_attenuation += lightToSurfaceAngle - coneAngle;
		}
		float attenuation = 1.0f / ((dist));
		
		vec3 lightColor = g_SpotLights[i].Color.rgb * attenuation;

		float theta = dot(lightDir, -targetDir);
		float epsilon = g_SpotLights[i].Angle - g_SpotLights[i].OuterAngle;
		float intensity = 10 * clamp((theta - g_SpotLights[i].OuterAngle) / epsilon, 0.0, 1.0);

		if(theta > g_SpotLights[i].OuterAngle)
		{
			c += CalcLight(normalize(lightDir), lightColor, viewDir, normal, color, specular, intensity);
		}
	}

	g_OutColor = vec4(c, 1.0f);
}
#endif