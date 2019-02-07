#pragma once
#include <Graphics/Shaders/ShaderProgram.h>

class CustomMaterial
{
public:
	CustomMaterial(CustomMaterial&& other) = delete;
	CustomMaterial(const CustomMaterial& other) = delete;
	CustomMaterial& operator=(CustomMaterial&& other) = delete;
	CustomMaterial& operator=(const CustomMaterial& other) = delete;

	CustomMaterial();
	CustomMaterial(ShaderProgram* pProgram);
	~CustomMaterial();

	virtual void Bind() const noexcept = 0;
	virtual void Unbind() const noexcept = 0;

protected:
	void SetProgram(ShaderProgram* pProgram) noexcept;
	const ShaderProgram* GetProgram() const noexcept;

private:
	ShaderProgram* m_pProgram;
};

inline void CustomMaterial::SetProgram(ShaderProgram* pProgram) noexcept
{
	m_pProgram = pProgram;
}

inline const ShaderProgram* CustomMaterial::GetProgram() const noexcept
{
	assert(m_pProgram != nullptr);
	return m_pProgram;
}