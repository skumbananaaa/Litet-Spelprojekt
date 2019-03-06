#pragma once
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>
#include "MaterialBase.h"
#include <IO/ResourceHandler.h>

class API Material
{
private:
	struct StencilFace
	{
		StencilOp StencilFail = STENCIL_OP_KEEP;
		StencilOp DepthFail = STENCIL_OP_KEEP;
		StencilOp DepthPass = STENCIL_OP_REPLACE;
	};

	friend class ResourceHandler;

public:
	virtual void Bind(const Framebuffer* pGBuffer) const noexcept;
	virtual void Unbind() const noexcept;

	void SetLightBuffer(const UniformBuffer* pLightBuffer) const noexcept;
	void SetCameraBuffer(const UniformBuffer* pCameraBuffer) const noexcept;
	void SetMaterialBuffer(const UniformBuffer* pMaterialBuffer) const noexcept;
	void SetExtensionBuffer(const UniformBuffer* pExtensionBuffer) const noexcept;
	void SetShadowBuffer(const UniformBuffer* pShadowBuffer) const noexcept;

	void SetStencilTest(bool enable, Func func = FUNC_ALWAYS, uint8 mask = 0xff, uint8 ref = 1, uint8 value = 0xff) const noexcept;
	void SetStencilOp(StencilOp sFail, StencilOp dFail, StencilOp dPass) const noexcept;
	void SetFrontFaceStencilOp(StencilOp sFail, StencilOp dFail, StencilOp dPass) const noexcept;
	void SetBackFaceStencilOp(StencilOp sFail, StencilOp dFail, StencilOp dPass) const noexcept;
	void SetCullMode(CULL_MODE mode) const noexcept;
	void SetLevelClipPlane(const glm::vec4& clipPlane) const noexcept;
	void SetIsReflectable(bool isReflectable) noexcept;

	const Texture2D* GetDiffuseMap() const noexcept;
	const glm::vec4& GetColor() const noexcept;
	const glm::vec4& GetLevelClipPlane() const noexcept;
	float GetSpecular() const noexcept;
	CULL_MODE GetCullMode() const noexcept;

	bool HasDiffuseMap() const noexcept;
	bool IncludeInDepthPrePass() const noexcept;
	bool IsReflectable() const noexcept;

protected:
	Material();
	Material(int32 shader);
	virtual ~Material();

	void SetProgram(int32 shader) noexcept;
	void SetIncludeInDepthPrePass(bool include) noexcept;

private:
	void SetDiffuseMap(const Texture2D* const pTexture) noexcept;
	void SetColor(const glm::vec4& color) noexcept;
	void SetSpecular(float specular) noexcept;

private:
	const ShaderProgram* m_pProgram;

	struct
	{
		mutable const UniformBuffer* pLightBuffer = nullptr;
		mutable const UniformBuffer* pCameraBuffer = nullptr;
		mutable const UniformBuffer* pMaterialBuffer = nullptr;
		mutable const UniformBuffer* pExtensionBuffer = nullptr;
		mutable const UniformBuffer* pShadowBuffer = nullptr;
		const Texture2D* pDiffuseMap = nullptr;
		const Texture2D* pSpecularMap = nullptr;
		glm::vec4 Color = glm::vec4(0.0f);
		float Specular = 256.0f;
	} m_Data;

	mutable struct
	{
		bool StencilTest = false;
		bool DepthPrePass = true;
		bool IsReflectable = false;
		Func StencilFunc = FUNC_ALWAYS;
		StencilFace Front;
		StencilFace Back;
		uint8 StencilMask = 0;
		uint8 StencilRef = 1;
		uint8 StencilValue = 0xff;
		glm::vec4 ClipPlane = glm::vec4(0.0f);
		CULL_MODE CullMode = CULL_MODE_BACK;
	} m_PipelineState;
};

inline bool Material::HasDiffuseMap() const noexcept
{
	return m_Data.pDiffuseMap != nullptr;
}

inline bool Material::IncludeInDepthPrePass() const noexcept
{
	return m_PipelineState.DepthPrePass;
}

inline bool Material::IsReflectable() const noexcept
{
	return m_PipelineState.IsReflectable;
}

inline void Material::SetProgram(int32 shader) noexcept
{
	m_pProgram = ResourceHandler::GetShader(shader);
}

inline void Material::SetIncludeInDepthPrePass(bool include) noexcept
{
	m_PipelineState.DepthPrePass = include;
}

inline void Material::SetDiffuseMap(const Texture2D* const pTexture) noexcept
{
	m_Data.pDiffuseMap = pTexture;
}

inline void Material::SetStencilTest(bool enable, Func func, uint8 mask, uint8 ref, uint8 value) const noexcept
{
	m_PipelineState.StencilTest = enable;
	m_PipelineState.StencilFunc = func;
	m_PipelineState.StencilMask = mask;
	m_PipelineState.StencilRef = ref;
	m_PipelineState.StencilValue = value;
}

inline void Material::SetStencilOp(StencilOp sFail, StencilOp dFail, StencilOp dPass) const noexcept
{
	m_PipelineState.Front.StencilFail = sFail;
	m_PipelineState.Front.DepthFail = dFail;
	m_PipelineState.Front.DepthPass = dPass;
	m_PipelineState.Back = m_PipelineState.Front;
}

inline void Material::SetFrontFaceStencilOp(StencilOp sFail, StencilOp dFail, StencilOp dPass) const noexcept
{
	m_PipelineState.Front.StencilFail = sFail;
	m_PipelineState.Front.DepthFail = dFail;
	m_PipelineState.Front.DepthPass = dPass;
}

inline void Material::SetBackFaceStencilOp(StencilOp sFail, StencilOp dFail, StencilOp dPass) const noexcept
{
	m_PipelineState.Back.StencilFail = sFail;
	m_PipelineState.Back.DepthFail = dFail;
	m_PipelineState.Back.DepthPass = dPass;
}

inline void Material::SetCullMode(CULL_MODE mode) const noexcept
{
	m_PipelineState.CullMode = mode;
}

inline void Material::SetLevelClipPlane(const glm::vec4& clipPlane) const noexcept
{
	m_PipelineState.ClipPlane = clipPlane;
}

inline void Material::SetIsReflectable(bool isReflectable) noexcept
{
	m_PipelineState.IsReflectable = isReflectable;
}

inline CULL_MODE Material::GetCullMode() const noexcept
{
	return m_PipelineState.CullMode;
}

inline const Texture2D* Material::GetDiffuseMap() const noexcept
{
	return m_Data.pDiffuseMap;
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

inline const glm::vec4& Material::GetLevelClipPlane() const noexcept
{
	return m_PipelineState.ClipPlane;
}
