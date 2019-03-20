#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>

constexpr uint32 WATER_QUAD_VERTICES_PER_SQUARE = 3 * 2;

struct WaterVertex
{
	glm::vec2 Position;
	glm::vec4 Indicators;

public:
	WaterVertex() {};
	WaterVertex(const glm::vec2& p, const glm::vec4& i)
	{
		Position = p;
		Indicators = i;
	};

	inline bool operator==(const WaterVertex& rs) const
	{
		return (Position == rs.Position) && (Indicators == rs.Indicators);
	};
};


class API WaterQuad : public IndexedMesh
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

private:
	WaterQuad(WaterQuad&& other) = delete;
	WaterQuad(const WaterQuad& other) = delete;
	WaterQuad& operator=(WaterQuad&& other) = delete;
	WaterQuad& operator=(const WaterQuad& other) = delete;

	WaterQuad(const void* const pVertices, const uint32* const pIndices, uint32 numVertices, uint32 numIndices) noexcept;
	virtual ~WaterQuad();

	virtual void Construct() override;

public:
	static WaterQuad* CreateWaterQuad(const glm::vec2& pos, const glm::vec2& centerHoleDim, float scale, uint32 gridDiameter) noexcept;

private:
	static void StoreGridSquare(float col, float row, float scale, std::vector<WaterVertex>& vertices) noexcept;
	static void CalculateCornerPositions(float col, float row, float scale, glm::vec2* cornerPositions) noexcept;
	static void StoreTriangle(glm::vec2 cornerPositions[4], std::vector<WaterVertex>& vertices, bool left) noexcept;
	static glm::vec4 GetIndicators(uint32 currentVertex, glm::vec2 cornerPositions[4], uint32 vertex0, uint32 vertex1) noexcept;
	static glm::vec4 GetIndicators(const glm::vec2& currentPosition, const glm::vec2& v0Position, const glm::vec2& v1Position) noexcept;

	static WaterVertex& FindLargestX(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2) noexcept;
	static WaterVertex& FindLargestZ(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2) noexcept;
	static WaterVertex& FindSmallestX(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2) noexcept;
	static WaterVertex& FindSmallestZ(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2) noexcept;
	static WaterVertex& FindCorner(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2, WaterVertex& vX, WaterVertex& vZ) noexcept;
};

inline WaterVertex& WaterQuad::FindLargestX(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2) noexcept
{
	glm::vec2 compare(-FLT_MAX, -FLT_MAX);
	WaterVertex* result = nullptr;

	if (v0.Position.x > compare.x)
	{
		compare = v0.Position;
		result = &v0;
	}

	if (v1.Position.x > compare.x)
	{
		compare = v1.Position;
		result = &v1;
	}

	if (v2.Position.x > compare.x)
	{
		compare = v2.Position;
		result = &v2;
	}

	return *result;
}

inline WaterVertex& WaterQuad::FindLargestZ(WaterVertex & v0, WaterVertex & v1, WaterVertex & v2) noexcept
{
	glm::vec2 compare(-FLT_MAX, -FLT_MAX);
	WaterVertex* result = nullptr;

	if (v0.Position.y > compare.y)
	{
		compare = v0.Position;
		result = &v0;
	}

	if (v1.Position.y > compare.y)
	{
		compare = v1.Position;
		result = &v1;
	}

	if (v2.Position.y > compare.y)
	{
		compare = v2.Position;
		result = &v2;
	}

	return *result;
}

inline WaterVertex& WaterQuad::FindSmallestX(WaterVertex & v0, WaterVertex & v1, WaterVertex & v2) noexcept
{
	glm::vec2 compare(FLT_MAX, FLT_MAX);
	WaterVertex* result = nullptr;

	if (v0.Position.x < compare.x)
	{
		compare = v0.Position;
		result = &v0;
	}

	if (v1.Position.x < compare.x)
	{
		compare = v1.Position;
		result = &v1;
	}

	if (v2.Position.x < compare.x)
	{
		compare = v2.Position;
		result = &v2;
	}

	return *result;
}

inline WaterVertex& WaterQuad::FindSmallestZ(WaterVertex & v0, WaterVertex & v1, WaterVertex & v2) noexcept
{
	glm::vec2 compare(FLT_MAX, FLT_MAX);
	WaterVertex* result = nullptr;

	if (v0.Position.y < compare.y)
	{
		compare = v0.Position;
		result = &v0;
	}

	if (v1.Position.y < compare.y)
	{
		compare = v1.Position;
		result = &v1;
	}

	if (v2.Position.y < compare.y)
	{
		compare = v2.Position;
		result = &v2;
	}

	return *result;
}

inline WaterVertex& WaterQuad::FindCorner(WaterVertex& v0, WaterVertex& v1, WaterVertex& v2, WaterVertex& vX, WaterVertex& vZ) noexcept
{
	if (v0.Position != vX.Position && v0.Position != vZ.Position)
	{
		return v0;
	}
	else if (v1.Position != vX.Position && v1.Position != vZ.Position)
	{
		return v1;
	}
	else if (v2.Position != vX.Position && v2.Position != vZ.Position)
	{
		return v2;
	}

	return v0;
}
