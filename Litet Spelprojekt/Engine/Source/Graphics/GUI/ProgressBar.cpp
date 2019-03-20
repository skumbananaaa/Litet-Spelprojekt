#include <EnginePch.h>
#include <Graphics/GUI/ProgressBar.h>

ProgressBar::ProgressBar(float x, float y, float width, float height) : GUIObject(x, y, width, height),
	m_Percentage(0),
	m_ProgressColor(0, 0.776, 0.878, 1.0F)
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
		if (!IsRealtimeRendered())
		{
			RequestRepaint();
		}
		for (IProgressListener* listener : m_ProgressListeners)
		{
			listener->OnProgressChange(this, percentage);
		}
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
		if (!IsRealtimeRendered())
		{
			RequestRepaint();
		}
	}
}

void ProgressBar::BeginProgress() noexcept
{
	AddRealTimeRenderer();
}

void ProgressBar::EndProgress() noexcept
{
	RemoveRealTimeRenderer();
}

void ProgressBar::AddProgressListener(IProgressListener * listener)
{
	if (!Contains<IProgressListener>(m_ProgressListeners, listener))
	{
		m_ProgressListeners.push_back(listener);
	}
	else
	{
		std::cout << "IProgressListener already added" << std::endl;
	}
}

void ProgressBar::RemoveProgressListener(IProgressListener * listener)
{
	int32 counter = 0;
	for (IProgressListener* object : m_ProgressListeners)
	{
		if (object == listener)
		{
			m_ProgressListeners.erase(m_ProgressListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void ProgressBar::OnRender(GUIContext* context)
{
	GUIObject::OnRender(context);
	if (!IsRealtimeRendered())
	{
		RenderProgress(context);
	}
}

void ProgressBar::RenderRealTime(GUIContext* context, float x, float y)
{
	RenderProgress(context, x, y);
}

void ProgressBar::RenderProgress(GUIContext* context, float x, float y) noexcept
{
	context->RenderTexture(GetDefaultTexture(), x, y, GetWidth() * m_Percentage, GetHeight(), GetProgressColor());
}

void ProgressBar::PrintName() const
{
	std::cout << "ProgressBar";
}
