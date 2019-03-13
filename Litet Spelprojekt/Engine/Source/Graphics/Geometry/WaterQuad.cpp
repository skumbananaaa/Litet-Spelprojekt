#include <EnginePch.h>
#include <Graphics/Geometry/WaterQuad.h>
#include <Graphics/Renderers/GLContext.h>

WaterQuad::WaterQuad(const void* const pVertices, const uint32* const pIndices, uint32 numVertices, uint32 numIndices) noexcept :
	IndexedMesh(pVertices, pIndices, numVertices, numIndices)
{
}

WaterQuad::~WaterQuad()
{
}

void WaterQuad::Construct()
{
	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glGenBuffers(1, &m_VBO));
	GL_CALL(glGenBuffers(1, &m_IBO));
	GL_CALL(glGenBuffers(1, &m_InstanceBuffer));

	GL_CALL(glBindVertexArray(m_VAO));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(WaterVertex), m_pVertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), m_pIndices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//Indicators
	GL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (void*)(2 * sizeof(float))));
	GL_CALL(glEnableVertexAttribArray(1));

	m_NumReservedInstances = 1;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumReservedInstances * sizeof(InstanceData), nullptr, GL_STATIC_DRAW));

	//Instance model matrix
	GL_CALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0));
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribDivisor(2, 1));
	GL_CALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(3));
	GL_CALL(glVertexAttribDivisor(3, 1));
	GL_CALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(2 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(4));
	GL_CALL(glVertexAttribDivisor(4, 1));
	GL_CALL(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(3 * sizeof(glm::vec4))));
	GL_CALL(glEnableVertexAttribArray(5));
	GL_CALL(glVertexAttribDivisor(5, 1));

	//Instance inversemodel matrix
	//GL_CALL(glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(4 * sizeof(glm::vec4))));
	//GL_CALL(glEnableVertexAttribArray(8));
	//GL_CALL(glVertexAttribDivisor(8, 1));
	//GL_CALL(glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(5 * sizeof(glm::vec4))));
	//GL_CALL(glEnableVertexAttribArray(9));
	//GL_CALL(glVertexAttribDivisor(9, 1));
	//GL_CALL(glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(6 * sizeof(glm::vec4))));
	//GL_CALL(glEnableVertexAttribArray(10));
	//GL_CALL(glVertexAttribDivisor(10, 1));
	//GL_CALL(glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(7 * sizeof(glm::vec4))));
	//GL_CALL(glEnableVertexAttribArray(11));
	//GL_CALL(glVertexAttribDivisor(11, 1));

	////Instance direction matrix
	//GL_CALL(glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(8 * sizeof(glm::vec4))));
	//GL_CALL(glEnableVertexAttribArray(12));
	//GL_CALL(glVertexAttribDivisor(12, 1));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	free(m_pVertices);
	m_pVertices = nullptr;
	DeleteArrSafe(m_pIndices);
}

WaterQuad* WaterQuad::CreateWaterQuad(glm::vec2& pos, const glm::vec2 centerHoleDim, float scale, uint32 gridDiameter) noexcept
{
	uint32 numVertices = gridDiameter * gridDiameter * WATER_QUAD_VERTICES_PER_SQUARE;
	std::vector<WaterVertex> vertices;

	glm::vec2 position = pos + glm::vec2(0.5f, 0.5f) + glm::vec2(1.0f, 0.0f) * (1.0f - scale);

	float gridRadius = (float)gridDiameter / 2.0f;
	float rowStart = position.x - gridRadius * scale;
	float colStart = position.y - gridRadius * scale;
	float rowEnd = position.x + gridRadius * scale;
	float colEnd = position.y + gridRadius * scale;

	glm::vec2 minHoleCoords = position - centerHoleDim / 2.0f + glm::vec2(-1.0f);
	glm::vec2 maxHoleCoords = position + centerHoleDim / 2.0f + glm::vec2(-2.0f);

	for (float row = rowStart; row < rowEnd; row += scale)
	{
		for (float col = colStart; col < colEnd; col += scale)
		{
			//if (row < minHoleCoords.x || row > maxHoleCoords.x || col < minHoleCoords.y || col > maxHoleCoords.y)
			{
				StoreGridSquare(col, row, scale, vertices);
			}
		}
	}

	WaterVertex* arrVertices = new WaterVertex[numVertices];
	memcpy(arrVertices, vertices.data(), sizeof(WaterVertex) * numVertices);

	uint32* arrIndices = new uint32[numVertices];
	for (uint32 i = 0; i < numVertices; i++) { arrIndices[i] = i; }

	return new WaterQuad(arrVertices, arrIndices, numVertices, numVertices);
}

void WaterQuad::StoreGridSquare(float col, float row, float scale, std::vector<WaterVertex>& vertices) noexcept
{
	glm::vec2 cornerPositions[4];
	CalculateCornerPositions(col, row, scale, cornerPositions);
	StoreTriangle(cornerPositions, vertices, true);
	StoreTriangle(cornerPositions, vertices, false);
}

void WaterQuad::CalculateCornerPositions(float col, float row, float scale, glm::vec2* cornerPositions) noexcept
{
	cornerPositions[0] = glm::vec2(col, row);
	cornerPositions[1] = glm::vec2(col, row + scale);
	cornerPositions[2] = glm::vec2(col + scale, row);
	cornerPositions[3] = glm::vec2(col + scale, row + scale);
}

void WaterQuad::StoreTriangle(glm::vec2 cornerPositions[4], std::vector<WaterVertex>& vertices, bool left) noexcept
{
	int index0 = left ? 0 : 2;
	int index1 = 1;
	int index2 = left ? 2 : 3;
	WaterVertex vertex = {};

	//Vertex 0
	{
		vertex.Position = cornerPositions[index0];
		vertex.Indicators = GetIndicators(index0, cornerPositions, index1, index2);
		vertices.push_back(vertex);
	}

	//Vertex 1
	{
		vertex.Position = cornerPositions[index1];
		vertex.Indicators = GetIndicators(index1, cornerPositions, index2, index0);
		vertices.push_back(vertex);
	}

	//Vertex 2
	{
		vertex.Position = cornerPositions[index2];
		vertex.Indicators = GetIndicators(index2, cornerPositions, index0, index1);
		vertices.push_back(vertex);
	}
}

glm::vec4 WaterQuad::GetIndicators(uint32 currentVertex, glm::vec2 cornerPositions[4], uint32 vertex0, uint32 vertex1) noexcept
{
	glm::vec2 currentVertexPos = cornerPositions[currentVertex];
	glm::vec2 vertex0Pos = cornerPositions[vertex0];
	glm::vec2 vertex1Pos = cornerPositions[vertex1];
	glm::vec2 offset0 = vertex0Pos - currentVertexPos;
	glm::vec2 offset1 = vertex1Pos - currentVertexPos;
	return glm::vec4(offset0.x, offset0.y, offset1.x, offset1.y);
}
