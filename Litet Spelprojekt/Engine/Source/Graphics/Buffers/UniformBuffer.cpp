#include <EnginePch.h>
#include "..\..\..\Include\Graphics\Buffers\UniformBuffer.h"

UniformBuffer::UniformBuffer(const void * pInitalData, uint32 count, uint32 stride)
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
		glDeleteBuffers(1, &m_Buffer);
		m_Buffer = 0;
	}
}

void UniformBuffer::UpdateData(const void* pData) noexcept 
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Count * m_Stride, pData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Create(const void* pInitalData, uint32 count, uint32 stride) noexcept
{
	glGenBuffers(1, &m_Buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
	
	glBufferData(GL_UNIFORM_BUFFER, count * stride, pInitalData, GL_STATIC_DRAW);
	m_Count = count;
	m_Stride = stride;

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
