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
	static WaterQuad* CreateWaterQuad(glm::vec2& pos, const glm::vec2 centerHoleDim, float scale, uint32 gridDiameter) noexcept;

private:
	static void StoreGridSquare(float col, float row, float scale, std::vector<WaterVertex>& vertices) noexcept;
	static void CalculateCornerPositions(float col, float row, float scale, glm::vec2* cornerPositions) noexcept;
	static void StoreTriangle(glm::vec2 cornerPositions[4], std::vector<WaterVertex>& vertices, bool left) noexcept;
	static glm::vec4 GetIndicators(uint32 currentVertex, glm::vec2 cornerPositions[4], uint32 vertex0, uint32 vertex1) noexcept;
};