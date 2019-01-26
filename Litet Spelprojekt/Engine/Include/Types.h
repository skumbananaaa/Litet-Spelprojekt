#pragma once

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef uint8 byte;

namespace glm
{
	template struct API vec<4, float, glm::packed_highp>;
	template struct API vec<3, float, glm::packed_highp>;
	template struct API vec<2, float, glm::packed_highp>;

	template struct API mat<4, 4, glm::f32, glm::packed_highp>;
}