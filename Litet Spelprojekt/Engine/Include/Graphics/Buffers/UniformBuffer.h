#pragma once
#include <EnginePch.h>

class UniformBuffer
{
	friend class GLContext;

public:
	UniformBuffer(UniformBuffer&& other) = delete;
	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer& operator=(UniformBuffer&& other) = delete;
	UniformBuffer& operator=(const UniformBuffer& other) = delete;

	UniformBuffer(const void* pInitalData, unsigned int count, unsigned int stride);
	~UniformBuffer();

	unsigned int GetCount() const noexcept;
	unsigned int GetStride() const noexcept;
	void UpdateData(const void* pData) noexcept;

private:
	void Create(const void* pInitalData, unsigned int count, unsigned int stride) noexcept;

private:
	unsigned int m_Buffer;
	unsigned int m_Count;
	unsigned int m_Stride;
};

inline unsigned int UniformBuffer::GetCount() const noexcept
{
	return m_Count;
}

inline unsigned int UniformBuffer::GetStride() const noexcept
{
	return m_Stride;
}