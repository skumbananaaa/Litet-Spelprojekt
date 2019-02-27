#include <EnginePch.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

Framebuffer::Framebuffer(const FramebufferDesc& desc)
	: m_ppColor(),
	m_pDepthStencil(nullptr),
	m_NumColorAttachments(0),
	m_Framebuffer(0),
	m_IsOwner(true),
	m_Width(0),
	m_Height(0),
	m_Samples(0)
{
	for (uint32 i = 0; i < 8; i++)
	{
		m_ppColor[i] = nullptr;
	}

	Create(desc);
}

Framebuffer::Framebuffer(Texture** ppColor, uint32 numTextures, Texture* pDepthStencil)
	: m_ppColor(),
	m_pDepthStencil(nullptr),
	m_NumColorAttachments(0),
	m_Framebuffer(0),
	m_IsOwner(false),
	m_Width(0),
	m_Height(0),
	m_Samples(0)
{
	for (uint32 i = 0; i < 8; i++)
	{
		m_ppColor[i] = nullptr;
	}

	Create(ppColor, numTextures, pDepthStencil);
}

Framebuffer::~Framebuffer()
{
	if (m_IsOwner)
	{
		for (uint32 i = 0; i < m_NumColorAttachments; i++)
		{
			DeleteSafe(m_ppColor[i]);
		}

		DeleteSafe(m_pDepthStencil);
	}

	if (glIsFramebuffer(m_Framebuffer))
	{
		GL_CALL(glDeleteFramebuffers(1, &m_Framebuffer));
		m_Framebuffer = 0;
	}
}

void Framebuffer::Create(const FramebufferDesc& desc)
{
	TextureDesc textureDesc = {};
	textureDesc.GenerateMips = false;
	textureDesc.Width = desc.Width;
	textureDesc.Height = desc.Height;
	textureDesc.Samples = desc.Samples;

	m_NumColorAttachments = desc.NumColorAttachments;
	for (uint32 i = 0; i < desc.NumColorAttachments; i++)
	{
		if (desc.ColorAttchmentFormats[i] != TEX_FORMAT_UNKNOWN)
		{
			textureDesc.Format = desc.ColorAttchmentFormats[i];
			m_ppColor[i] = new Texture2D(nullptr, textureDesc, desc.SamplingParams);
		}
	}

	if (desc.DepthStencilFormat != TEX_FORMAT_UNKNOWN)
	{
		textureDesc.Format = desc.DepthStencilFormat;
		m_pDepthStencil = new Texture2D(nullptr, textureDesc, desc.SamplingParams);
	}

	m_Width = desc.Width;
	m_Height = desc.Height;
	m_Samples = desc.Samples;

	CreateFramebuffer();
}

void Framebuffer::Create(Texture** ppColor, uint32 numTextures, Texture* pDepthStencil)
{
	if (numTextures > 8)
	{
		std::cout << "Error cannot have more than 8 textures in a framebuffer" << std::endl;
		return;
	}

	if (pDepthStencil != nullptr)
	{
		m_Width = pDepthStencil->GetWidth();
		m_Height = pDepthStencil->GetHeight();
		m_Samples = pDepthStencil->GetSamples();

		m_pDepthStencil = pDepthStencil;
	}
	else if (ppColor[0] != nullptr)
	{
		m_Width = ppColor[0]->GetWidth();
		m_Height = ppColor[0]->GetHeight();
		m_Samples = ppColor[0]->GetSamples();
	}
	else
	{
		return;
	}

	for (uint32 i = 0; i < numTextures; i++)
	{
		m_ppColor[i] = ppColor[i];
		if (ppColor[i]->GetWidth() != m_Width || ppColor[i]->GetHeight() != m_Height || m_Samples != ppColor[i]->GetSamples())
		{
			std::cout << "Error: All textures in a framebuffer needs to have the same size and samples" << std::endl;
			return;
		}
	}

	m_NumColorAttachments = numTextures;
	CreateFramebuffer();
}

void Framebuffer::CreateFramebuffer()
{
	GL_CALL(glGenFramebuffers(1, &m_Framebuffer));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));

	uint32 buf = 0;
	uint32 drawBuffers[8];
	for (uint32 i = 0; i < m_NumColorAttachments; i++)
	{
		if (m_ppColor[i] != nullptr)
		{
			drawBuffers[buf] = GL_COLOR_ATTACHMENT0 + buf;
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers[buf], m_ppColor[i]->GetType(), m_ppColor[i]->m_Texture, 0));

			buf++;
		}
	}

	GL_CALL(glDrawBuffers(buf, drawBuffers));

	if (m_pDepthStencil != nullptr)
	{
		if (m_pDepthStencil->GetFormat() == TEX_FORMAT_DEPTH)
		{
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_pDepthStencil->GetType(), m_pDepthStencil->m_Texture, 0));
		}
		else if (m_pDepthStencil->GetFormat() == TEX_FORMAT_DEPTH_STENCIL)
		{
			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_pDepthStencil->GetType(), m_pDepthStencil->m_Texture, 0));
		}
		else
		{
			std::cout << "Not a valid format for depth" << std::endl;
		}
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error: Could not create framebuffer. Status(" << std::to_string(status) << ") - ";

		switch (status)
		{
		case GL_FRAMEBUFFER_UNDEFINED: 
			std::cout << "GL_FRAMEBUFFER_UNDEFINED." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER." << std::endl;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE (Not all attachments has the same number of samples)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS." << std::endl;
			break;
		}
	}

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
