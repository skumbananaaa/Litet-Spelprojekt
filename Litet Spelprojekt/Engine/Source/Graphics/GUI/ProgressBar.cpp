#include <EnginePch.h>
#include <Graphics/GUI/ProgressBar.h>

ProgressBar::ProgressBar(float x, float y, float width, float height) : GUIObject(x, y, width, height),
	m_Percentage(0),
	m_ProgressColor(0.553F, 0.824F, 0.541F, 1.0F)
{
	SetBackgroundColor(glm::vec4(0.408F, 0.408F, 0.408F, 1.0F));
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::SetPercentage(float percentage)
{
	percentage = glm::clamp<float>(percentage, 0.0F, 1.0F);
	if (percentage != m_Percentage)
	{
		m_Percentage = percentage;
		RequestRepaint();
	}
}

float ProgressBar::GetPercentage() const noexcept
{
	return m_Percentage;
}

const glm::vec4& ProgressBar::GetProgressColor() const noexcept
{
	return m_ProgressColor;
}

void ProgressBar::SetProgressColor(const glm::vec4& color)
{
	if (m_ProgressColor != color)
	{
		m_ProgressColor = color;
		RequestRepaint();
	}
}

void ProgressBar::OnRender(GUIContext* context)
{
	GUIObject::OnRender(context);

	context->SetVertexQuadData(0, 0, GetWidth() * m_Percentage, GetHeight(), GetProgressColor());
	context->GetGraphicsContext()->SetTexture(GetDefaultTexture(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	context->GetGraphicsContext()->SetTexture(nullptr, 0);
}

void ProgressBar::PrintName() const
{
	std::cout << "ProgressBar";
}
