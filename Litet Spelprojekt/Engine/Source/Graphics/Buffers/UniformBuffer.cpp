#include <EnginePch.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Renderers/GLContext.h>

UniformBuffer::UniformBuffer(const void* pInitalData, uint32 count, uint32 stride)
	: m_Buffer(0),
	m_Count(0),
	m_Stride(0)
{
	Create(pInitalData, count, stride);
}

UniformBuffer::~UniformBuffer()
{
	if (glIsBuffer(m_Buffer))
	{
		GL_CALL(glDeleteBuffers(1, &m_Buffer));
		m_Buffer = 0;
	}
}

void UniformBuffer::UpdateData(const void* pData) noexcept 
{
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer));
	GL_CALL(glBufferData(GL_UNIFORM_BUFFER, m_Count * m_Stride, pData, GL_DYNAMIC_DRAW));
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::Create(const void* pInitalData, uint32 count, uint32 stride) noexcept
{
	GL_CALL(glGenBuffers(1, &m_Buffer));
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer));
	
	GL_CALL(glBufferData(GL_UNIFORM_BUFFER, count * stride, pInitalData, GL_DYNAMIC_DRAW));
	m_Count = count;
	m_Stride = stride;

	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
