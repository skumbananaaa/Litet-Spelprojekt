#pragma once
#include "Texture.h"

//Maximum amount of rendertargets usally is 8
struct FramebufferDesc
{
	TEX_FORMAT ColorAttchmentFormats[8];
	uint32 NumColorAttachments = 0;
	TEX_FORMAT DepthStencilFormat = TEX_FORMAT_UNKNOWN;
};

class API Framebuffer
{
public:
	Framebuffer(const FramebufferDesc& desc);
	Framebuffer(const Texture* texture);
	~Framebuffer();

	Texture* GetColorAttachment(uint32 index) const;
	Texture* GetDepthAttachment() const;
	uint32 GetColorAttachmentCount() const;

private:
	void Create(const FramebufferDesc& desc);
	void Create(const Texture* texture);

private:
	Texture* m_pColor[8];
	Texture* m_pDepth;
	uint32 m_NumColorAttachments;
	uint32 m_Framebuffer;
};


inline Texture * Framebuffer::GetColorAttachment(uint32 index) const
{
	return nullptr;
}

inline Texture * Framebuffer::GetDepthAttachment() const
{
	return nullptr;
}

inline uint32 Framebuffer::GetColorAttachmentCount() const
{
	return uint32();
}