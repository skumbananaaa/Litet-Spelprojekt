#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec4 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;
layout(binding = 2) uniform sampler2D g_DissolveMap;

layout(std140, binding = 0) uniform PerFrame
{
	mat4 g_ViewProjection;
	vec3 g_CameraPosition;
	float g_Padding;
	vec3 g_CameraLookAt;
};

layout(std140, binding = 1) uniform PerObject
{
	vec4 g_ClipPlane;
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
} 

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }

#define EPSILON 0.001f

void main()
{
	float minDissolve = 2.0f * ((snoise(fs_in.Position.xyz) / 2.0f) + 0.5f);

	//vec2 dissolveTexCoords = vec2((fs_in.Position.x + fs_in.Position.z) / 2.0f, fs_in.Position.y);
	//float minDissolve = texture(g_DissolveMap, dissolveTexCoords).r * 6.0f;

	//vec3 cameraForward = normalize(g_CameraLookAt.xyz - g_CameraPosition.xyz);

	float isNotUp = abs(dot(fs_in.Normal, vec3(0.0f, 1.0f, 0.0f)));
	float distanceToLookAt = length(g_CameraLookAt.xyz - fs_in.Position.xyz);

	vec3 toLookAt = normalize(g_CameraLookAt.xyz - fs_in.Position.xyz);
	float dotToLookAtNormal = dot(fs_in.Normal, toLookAt);

	//float dotToLookAtForward = dot(toLookAt, -cameraForward);
	//float dotForwardNormal = dot(cameraForward, fs_in.Normal);

	if (dotToLookAtNormal < 0.0)
	{
		discard;
	}
	else if (distanceToLookAt * ((dotToLookAtNormal / 2.0f) + 0.5f) < minDissolve && isNotUp < EPSILON)
	{
		discard;
	}

	//COLOR
	vec3 mappedColor = texture(g_Texture, fs_in.TexCoords).rgb * g_HasTexture;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasTexture);
	g_OutColor = vec4(uniformColor + mappedColor, 1.0f);
	
	//NORMAL
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	
	mat3 tbn = mat3(fs_in.Tangent, fs_in.Binormal, fs_in.Normal);
	mappedNormal = tbn * mappedNormal;

	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);
	normal = (normalize(normal) + vec3(1.0f)) * 0.5f;

	g_Normal = vec4(normal, 1.0f);
}