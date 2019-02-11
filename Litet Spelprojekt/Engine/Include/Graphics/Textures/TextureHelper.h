#pragma once
#include <EnginePch.h>

inline glm::vec4 RGBA2HSVA(const glm::vec4& rgba) noexcept
{
	float fH = 0;
	float fS = 0;
	float fV = 0;
	float fCMax = glm::max(glm::max(rgba.r, rgba.g), rgba.b);
	float fCMin = glm::min(glm::min(rgba.r, rgba.g), rgba.b);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) 
	{
		if (fCMax == rgba.r) 
		{
			fH = 60 * (fmod(((rgba.g - rgba.b) / fDelta), 6));
		}
		else if (fCMax == rgba.g) 
		{
			fH = 60 * (((rgba.b - rgba.r) / fDelta) + 2);
		}
		else if (fCMax == rgba.b) 
		{
			fH = 60 * (((rgba.r - rgba.g) / fDelta) + 4);
		}

		if (fCMax > 0) 
		{
			fS = fDelta / fCMax;
		}
		else
		{
			fS = 0;
		}

		fV = fCMax;
	}
	else 
	{
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if (fH < 0)
	{
		fH = 360 + fH;
	}

	return glm::vec4(fH, fS, fV, rgba.a);
}

inline glm::vec4 HSVA2RGBA (const glm::vec4& hsva) noexcept
{
	float fR = 0;
	float fG = 0;
	float fB = 0;
	float fC = hsva.z * hsva.y; // Chroma
	float fHPrime = fmod(hsva.x / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = hsva.z - fC;

	if (0 <= fHPrime && fHPrime < 1)
	{
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2)
	{
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3)
	{
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4)
	{
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5)
	{
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6)
	{
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else
	{
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;

	return glm::vec4(fR, fG, fB, hsva.w);
}
