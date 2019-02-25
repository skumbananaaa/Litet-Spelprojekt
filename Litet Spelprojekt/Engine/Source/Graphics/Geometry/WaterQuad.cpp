#include <EnginePch.h>
#include <Graphics/Geometry/WaterQuad.h>
#include <Graphics/Renderers/GLContext.h>

WaterQuad::WaterQuad(const void* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept :
	IndexedMesh(vertices, indices, numVertices, numIndices)
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
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(WaterVertex), m_Vertices, GL_STATIC_DRAW));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32), m_Indices, GL_STATIC_DRAW));

	//Position
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//Indicators
	GL_CALL(glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(WaterVertex), (void*)(2 * sizeof(float))));
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

	free(m_Vertices);
	m_Vertices = nullptr;
	DeleteArrSafe(m_Indices);
}

WaterQuad* WaterQuad::CreateWaterQuad(uint32 gridDiameter) noexcept
{
	uint32 numVertices = gridDiameter * gridDiameter * WATER_QUAD_VERTICES_PER_SQUARE;
	std::vector<WaterVertex> vertices;

	for (uint32 row = 0; row < gridDiameter; row++)
	{
		for (uint32 col = 0; col < gridDiameter; col++)
		{
			StoreGridSquare(col, row, vertices);
		}
	}

	WaterVertex* arrVertices = new WaterVertex[numVertices];
	memcpy(arrVertices, vertices.data(), sizeof(WaterVertex) * numVertices);

	uint32* arrIndices = new uint32[numVertices];
	for (uint32 i = 0; i < numVertices; i++) { arrIndices[i] = i; }

	return new WaterQuad(arrVertices, arrIndices, numVertices, numVertices);
}

void WaterQuad::StoreGridSquare(uint32 col, uint32 row, std::vector<WaterVertex>& vertices) noexcept
{
	glm::vec2 cornerPositions[4];
	CalculateCornerPositions(col, row, cornerPositions);
	StoreTriangle(cornerPositions, vertices, true);
	StoreTriangle(cornerPositions, vertices, false);
}

void WaterQuad::CalculateCornerPositions(uint32 col, uint32 row, glm::vec2* cornerPositions) noexcept
{
	cornerPositions[0] = glm::vec2(col, row);
	cornerPositions[1] = glm::vec2(col, row + 1);
	cornerPositions[2] = glm::vec2(col + 1, row);
	cornerPositions[3] = glm::vec2(col + 1, row + 1);
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
		vertex.Indicator0 = GetIndicators(index0, cornerPositions, index1);
		vertex.Indicator1 = GetIndicators(index0, cornerPositions, index2);
		vertices.push_back(vertex);
	}

	//Vertex 1
	{
		vertex.Position = cornerPositions[index1];
		vertex.Indicator0 = GetIndicators(index1, cornerPositions, index2);
		vertex.Indicator1 = GetIndicators(index1, cornerPositions, index0);
		vertices.push_back(vertex);
	}

	//Vertex 2
	{
		vertex.Position = cornerPositions[index2];
		vertex.Indicator0 = GetIndicators(index2, cornerPositions, index0);
		vertex.Indicator1 = GetIndicators(index2, cornerPositions, index1);
		vertices.push_back(vertex);
	}
}

glm::lowp_fvec2 WaterQuad::GetIndicators(uint32 currentVertex, glm::vec2 cornerPositions[4], uint32 vertex) noexcept
{
	glm::vec2 currentVertexPos = cornerPositions[currentVertex];
	glm::vec2 vertexPos = cornerPositions[vertex];
	glm::vec2 offset = vertexPos - currentVertexPos;
	return glm::lowp_fvec2(offset.x, offset.y);
}
