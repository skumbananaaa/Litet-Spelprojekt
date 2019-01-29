#pragma once
#include "Texture.h"

//Maximum amount of rendertargets usally is 8
struct FramebufferDesc
{
	TEX_FORMAT DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	TEX_FORMAT ColorAttchmentFormats[8];
	TextureParams SamplingParams = TextureParams();
	uint32 NumColorAttachments = 0;
	uint32 Width = 0;
	uint32 Height = 0;
	uint32 Samples = 1;
};

class API Framebuffer
{
	friend class GLContext;

public:
	Framebuffer(const FramebufferDesc& desc);
	Framebuffer(const Texture* texture);
	~Framebuffer();

	Texture* GetColorAttachment(uint32 index) const;
	Texture* GetDepthAttachment() const;
	uint32 GetColorAttachmentCount() const;
	uint32 GetWidth() const;
	uint32 GetHeight() const;

private:
	void Create(const FramebufferDesc& desc);
	void Create(const Texture* texture);
	void CreateFramebuffer();

private:
	Texture* m_ppColor[8];
	Texture* m_pDepth;
	uint32 m_NumColorAttachments;
	uint32 m_Framebuffer;
	uint32 m_Width;
	uint32 m_Height;
	uint32 m_Samples;
	bool m_IsOwner;
};


inline Texture* Framebuffer::GetColorAttachment(uint32 index) const
{
	assert(index < m_NumColorAttachments);
	return m_ppColor[index];
}

inline Texture* Framebuffer::GetDepthAttachment() const
{
	return m_pDepth;
}

inline uint32 Framebuffer::GetColorAttachmentCount() const
{
	return m_NumColorAttachments;
}

inline uint32 Framebuffer::GetWidth() const
{
	return m_Width;
}

inline uint32 Framebuffer::GetHeight() const
{
	return m_Height;
}
