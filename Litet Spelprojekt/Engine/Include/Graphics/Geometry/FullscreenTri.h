#pragma once
#include <EnginePch.h>

class API FullscreenTri
{
	friend class GLContext;

public:
	FullscreenTri(FullscreenTri&& other) = delete;
	FullscreenTri(const FullscreenTri& other) = delete;
	FullscreenTri& operator=(FullscreenTri&& other) = delete;
	FullscreenTri& operator=(const FullscreenTri& other) = delete;

	FullscreenTri() noexcept;
	~FullscreenTri();

private:
	void Create() noexcept;

public:
	uint32 m_VAO;
};