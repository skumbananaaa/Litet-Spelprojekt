#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>

StaticShadowCube::StaticShadowCube(const PointLight& light, const Scene& scene)
	: m_pCube(nullptr)
{
	Create(light, scene);
}

StaticShadowCube::~StaticShadowCube()
{
	DeleteSafe(m_pCube);
}

void StaticShadowCube::Create(const PointLight& light, const Scene& scene)
{
	{
		TextureDesc desc = {};
		desc.GenerateMips = false;
		desc.Format = TEX_FORMAT_DEPTH;
		desc.Width = SHADOW_SIZE;
		desc.Height = SHADOW_SIZE;
		desc.Samples = 1;

		m_pCube = TextureCube::CreateTextureCubeFromMemory(nullptr, desc);
	}

	{
		Framebuffer* ppFrameBuffers[6] = 
		{
			new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_POSITIVE_X),
			new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X),
			new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y),
			new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y),
			new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z),
			new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z),
		};

		for (uint32 i = 0; i < 6; i++)
		{
			DeleteSafe(ppFrameBuffers[i]);
		}

		std::cout << "Created ShadowCube" << std::endl;
	}
}
