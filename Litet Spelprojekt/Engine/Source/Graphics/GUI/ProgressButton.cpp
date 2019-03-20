#include <EnginePch.h>
#include <Graphics\GUI\ProgressButton.h>

ProgressButton::ProgressButton(float x, float y, float width, float height, const std::string& text) : Button(x, y, width, height, text),
	m_Percentage(0),
	m_TimeOfAnimation(0.0F),
	m_Timer(0.0F),
	m_ProgressColor(0, 0.776, 0.878, 1.0F)
{
	SetTextBuffered(true);
}

ProgressButton::~ProgressButton()
{

}


void ProgressButton::SetPercentage(float percentage)
{
	percentage = glm::clamp<float>(percentage, 0.0F, 1.0F);
	if (percentage != m_Percentage)
	{
		m_Percentage = percentage;
		RequestRepaint();
	}
}

float ProgressButton::GetPercentage() const noexcept
{
	return m_Percentage;
}

const glm::vec4& ProgressButton::GetProgressColor() const noexcept
{
	return m_ProgressColor;
}

void ProgressButton::SetProgressColor(const glm::vec4& color)
{
	if (m_ProgressColor != color)
	{
		m_ProgressColor = color;
		RequestRepaint();
	}
}

void ProgressButton::AddProgressButtonListener(IProgressButtonListener* listener)
{
	if (!Contains<IProgressButtonListener>(m_Listeners, listener))
	{
		m_Listeners.push_back(listener);
	}
	else
	{
		std::cout << "IProgressListener already added" << std::endl;
	}
}

void ProgressButton::RemoveProgressButtonListener(IProgressButtonListener* listener)
{
	int32 counter = 0;
	for (IProgressButtonListener* object : m_Listeners)
	{
		if (object == listener)
		{
			m_Listeners.erase(m_Listeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void ProgressButton::StartAnimation(float seconds) noexcept
{
	if (!IsRealtimeRendered())
	{
		m_TimeOfAnimation = seconds;
		AddRealTimeRenderer();
	}
}

void ProgressButton::OnUpdate(float dtS)
{
	if (m_TimeOfAnimation > 0)
	{
		m_Timer += dtS;
		m_Percentage = m_Timer / m_TimeOfAnimation;
		m_Percentage = glm::clamp<float>(m_Percentage, 0.0F, 1.0F);
		if (m_Timer >= m_TimeOfAnimation)
		{
			m_TimeOfAnimation = 0;
			m_Timer = 0;
			RemoveRealTimeRenderer();
			RequestRepaint();
			for (IProgressButtonListener* listener : m_Listeners)
			{
				listener->OnProgressAnimationEnd(this);
			}
		}
	}
}

void ProgressButton::OnRender(GUIContext* context)
{
	GUIObject::RenderBackgroundTexture(context);
	if (!IsRealtimeRendered())
	{
		RenderProgress(context);
		TextView::RenderText(context);
	}
}

void ProgressButton::RenderProgress(GUIContext* context, float x, float y) noexcept
{
	context->RenderTexture(GetDefaultTexture(), x, y, GetWidth() * m_Percentage, GetHeight(), GetProgressColor());
}

void ProgressButton::RenderRealTime(GUIContext* context, float x, float y)
{
	RenderProgress(context, x, y);
	Button::RenderTextBuffered(context, x, y);
}

void ProgressButton::PrintName() const
{
	std::cout << "ProgressButton" << std::endl;
}
