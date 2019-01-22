#pragma once
#include <EnginePch.h>

class API UniformBuffer
{
	friend class GLContext;

public:
	UniformBuffer(UniformBuffer&& other) = delete;
	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer& operator=(UniformBuffer&& other) = delete;
	UniformBuffer& operator=(const UniformBuffer& other) = delete;

	UniformBuffer(const void* pInitalData, uint32 count, uint32 stride);
	~UniformBuffer();

	uint32 GetCount() const noexcept;
	uint32 GetStride() const noexcept;
	void UpdateData(const void* pData) noexcept;

private:
	void Create(const void* pInitalData, uint32 count, uint32 stride) noexcept;

private:
	uint32 m_Buffer;
	uint32 m_Count;
	uint32 m_Stride;
};

inline uint32 UniformBuffer::GetCount() const noexcept
{
	return m_Count;
}

inline uint32 UniformBuffer::GetStride() const noexcept
{
	return m_Stride;
}