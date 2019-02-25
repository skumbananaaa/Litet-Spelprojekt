#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>

constexpr uint32 WATER_QUAD_VERTICES_PER_SQUARE = 3 * 2;

struct WaterVertex
{
	glm::vec2 Position;
	glm::lowp_fvec2 Indicator0;
	glm::lowp_fvec2 Indicator1;

public:
	WaterVertex() {};
	WaterVertex(const glm::vec2& p, const glm::lowp_fvec2& i0, const glm::lowp_fvec2& i1)
	{
		Position = p;
		Indicator0 = i0;
		Indicator1 = i1;
	};

	inline bool operator==(const WaterVertex& rs) const
	{
		return (Position == rs.Position) && (Indicator0 == rs.Indicator0) && (Indicator1 == rs.Indicator1);
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

	WaterQuad(const void* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept;
	virtual ~WaterQuad();

	virtual void Construct() override;

public:
	static WaterQuad* CreateWaterQuad(uint32 gridDiameter) noexcept;

private:
	static void StoreGridSquare(uint32 col, uint32 row, std::vector<WaterVertex>& vertices) noexcept;
	static void CalculateCornerPositions(uint32 col, uint32 row, glm::vec2* cornerPositions) noexcept;
	static void StoreTriangle(glm::vec2 cornerPositions[4], std::vector<WaterVertex>& vertices, bool left) noexcept;
	static glm::lowp_fvec2 GetIndicators(uint32 currentVertex, glm::vec2 cornerPositions[4], uint32 vertex) noexcept;
};