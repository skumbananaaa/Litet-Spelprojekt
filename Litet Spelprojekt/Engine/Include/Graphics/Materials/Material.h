#pragma once
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>
#include "MaterialBase.h"

#define DIFFUSE_MAP_BINDING_SLOT 0
#define NORMAL_MAP_BINDING_SLOT 1
#define SPECULAR_MAP_BINDING_SLOT 2

#define CAMERA_BUFFER_BINDING_SLOT 0
#define LIGHT_BUFFER_BINDING_SLOT 1
#define MATERIAL_BUFFER_BINDING_SLOT 2

class API Material
{
	friend class ResourceHandler;

public:
	Material();
	Material(ShaderProgram* pProgram);
	~Material();

	virtual void Bind(const Framebuffer* pGBuffer) const noexcept;
	virtual void Unbind() const noexcept;

	void SetLightBuffer(const UniformBuffer* pLightBuffer) const noexcept;
	void SetCameraBuffer(const UniformBuffer* pCameraBuffer) const noexcept;
	void SetMaterialBuffer(const UniformBuffer* pMaterialBuffer) const noexcept;

	void SetCullMode(CULL_MODE mode) noexcept;
	void EnableClipPlane(bool enable, uint32 index) noexcept;

	const Texture2D* GetNormalMap() const noexcept;
	const Texture2D* GetDiffuseMap() const noexcept;
	const Texture2D* GetSpecularMap() const noexcept;
	const glm::vec4& GetColor() const noexcept;
	float GetSpecular() const noexcept;
	CULL_MODE GetCullMode() const noexcept;

	bool HasDiffuseMap() const noexcept;
	bool HasNormalMap() const noexcept;
	bool HasSpecularMap() const noexcept;
	bool ClipPlaneEnabled(uint32 index) const noexcept;

protected:
	void SetProgram(ShaderProgram* pProgram) noexcept;

private:
	void SetDiffuseMap(const Texture2D* const pTexture) noexcept;
	void SetNormalMap(const Texture2D* const pNormalMap) noexcept;
	void SetSpecularMap(const Texture2D* const pNormalMap) noexcept;
	void SetColor(const glm::vec4& color) noexcept;
	void SetSpecular(float specular) noexcept;

private:
	ShaderProgram* m_pProgram;

	struct
	{
		mutable const UniformBuffer* pLightBuffer = nullptr;
		mutable const UniformBuffer* pCameraBuffer = nullptr;
		mutable const UniformBuffer* pMaterialBuffer = nullptr;
		const Texture2D* pDiffuseMap = nullptr;
		const Texture2D* pNormalMap = nullptr;
		const Texture2D* pSpecularMap = nullptr;
		glm::vec4 Color = glm::vec4(0.0f);
		float Specular = 256.0f;
	} m_Data;

	struct
	{
		CULL_MODE CullMode = CULL_MODE_BACK;
		bool ClipPlanesEnabled[NUM_CLIP_DISTANCES];
	} m_PipelineState;
};

inline bool Material::HasDiffuseMap() const noexcept
{
	return m_Data.pDiffuseMap != nullptr;
}

inline bool Material::HasNormalMap() const noexcept
{
	return m_Data.pNormalMap != nullptr;
}

inline bool Material::HasSpecularMap() const noexcept
{
	return m_Data.pSpecularMap != nullptr;
}

inline bool Material::ClipPlaneEnabled(uint32 index) const noexcept
{
	assert(index < NUM_CLIP_DISTANCES);
	return m_PipelineState.ClipPlanesEnabled[index];
}

inline void Material::SetProgram(ShaderProgram* pProgram) noexcept
{
	m_pProgram = pProgram;
}

inline void Material::SetDiffuseMap(const Texture2D* const pTexture) noexcept
{
	m_Data.pDiffuseMap = pTexture;
}

inline void Material::SetNormalMap(const Texture2D* const pNormalMap) noexcept
{
	m_Data.pNormalMap = pNormalMap;
}

inline void Material::SetSpecularMap(const Texture2D* const pSpecularMap) noexcept
{
	m_Data.pSpecularMap = pSpecularMap;
}

inline void Material::SetCullMode(CULL_MODE mode) noexcept
{
	m_PipelineState.CullMode = mode;
}

inline CULL_MODE Material::GetCullMode() const noexcept
{
	return m_PipelineState.CullMode;
}

inline void Material::EnableClipPlane(bool enable, uint32 index) noexcept
{
	assert(index < NUM_CLIP_DISTANCES);
	m_PipelineState.ClipPlanesEnabled[index] = enable;
}

inline const Texture2D* Material::GetDiffuseMap() const noexcept
{
	return m_Data.pDiffuseMap;
}

inline const Texture2D* Material::GetNormalMap() const noexcept
{
	return m_Data.pNormalMap;
}

inline const Texture2D* Material::GetSpecularMap() const noexcept
{
	return m_Data.pSpecularMap;
}

inline void Material::SetColor(const glm::vec4& color) noexcept
{
	m_Data.Color = color;
}

inline void Material::SetSpecular(float specular) noexcept
{
	m_Data.Specular = specular;
}

inline float Material::GetSpecular() const noexcept
{
	return m_Data.Specular;
}

inline const glm::vec4& Material::GetColor() const noexcept
{
	return m_Data.Color;
}
