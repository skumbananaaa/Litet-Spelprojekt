#include <EnginePch.h>
#include <Graphics/Textures/ShadowMapsCube.h>

StaticShadowCube::StaticShadowCube()
	: m_pCube(nullptr)
{
	Create();
}

StaticShadowCube::~StaticShadowCube()
{
	DeleteSafe(m_pCube);
}

void StaticShadowCube::Create()
{
	{
		TextureDesc desc = {};
		desc.GenerateMips = false;
		desc.Format = TEX_FORMAT_DEPTH;
		desc.Width = SHADOW_SIZE;
		desc.Height = SHADOW_SIZE;
		desc.Samples = 1;

		m_pCube = TextureCube::CreateTextureCubeFromMemory(nullptr, desc);

		std::cout << "Created ShadowCube" << std::endl;

		Framebuffer* ppFrameBuffers[6] = 
		{
			/*new Framebuffer(),*/nullptr,
			/*new Framebuffer(),*/nullptr,
			/*new Framebuffer(),*/nullptr,
			/*new Framebuffer(),*/nullptr,
			/*new Framebuffer(),*/nullptr,
			/*new Framebuffer(),*/nullptr,
		};

		for (uint32 i = 0; i < 6; i++)
		{
			DeleteSafe(ppFrameBuffers[i]);
		}
	}
}
