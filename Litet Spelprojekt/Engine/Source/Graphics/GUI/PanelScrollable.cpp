#include <EnginePch.h>
#include <Graphics/GUI/PanelScrollable.h>

#define SLIDER_SIZE 20

PanelScrollable::PanelScrollable(float x, float y, float width, float height, float clientWidth, float clientHeight) : Panel(x, y, width, height),
	m_pFrameBufferClientArea(nullptr),
	m_ClientOffset(0, 0)
{
	SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);

	m_pSliderVertical = new Slider(x + width - SLIDER_SIZE, y + SLIDER_SIZE, SLIDER_SIZE, height - SLIDER_SIZE);
	m_pSliderHorizontal = new Slider(x, y, width - SLIDER_SIZE, SLIDER_SIZE);
	SetClientSize(clientWidth, clientHeight);
}

PanelScrollable::~PanelScrollable()
{
	delete m_pFrameBufferClientArea;
}

float PanelScrollable::GetClientWidth() const noexcept
{
	return m_pFrameBufferClientArea->GetWidth();
}

float PanelScrollable::GetClientHeight() const noexcept
{
	return m_pFrameBufferClientArea->GetHeight();
}

void PanelScrollable::SetVisible(bool visible) noexcept
{
	Panel::SetVisible(visible);
	if (visible)
	{
		m_pSliderVertical->SetVisible(GetClientHeight() > GetHeight());
		m_pSliderHorizontal->SetVisible(GetClientWidth() > GetWidth());
	}
	else
	{
		m_pSliderVertical->SetVisible(false);
		m_pSliderHorizontal->SetVisible(false);
	}
}


void PanelScrollable::SetClientSize(float width, float height)
{
	if (m_pFrameBufferClientArea)
	{
		delete m_pFrameBufferClientArea;
	}

	FramebufferDesc desc;
	desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
	desc.SamplingParams = TextureParams();
	desc.NumColorAttachments = 1;
	desc.Width = static_cast<uint32>(width < GetWidth() ? GetWidth() : width);
	desc.Height = static_cast<uint32>(height < GetHeight() ? GetHeight() : height);

	m_pFrameBufferClientArea = new Framebuffer(desc);

	m_pSliderVertical->SetVisible(desc.Height > GetHeight());
	m_pSliderHorizontal->SetVisible(desc.Width > GetWidth());

	m_pSliderVertical->SetPosition(GetX() + GetWidth() - SLIDER_SIZE, GetY());
	m_pSliderVertical->SetSize(SLIDER_SIZE, GetHeight());

	if (m_pSliderVertical->IsVisible() && !m_pSliderHorizontal->IsVisible())
	{
		m_pSliderVertical->SetPosition(GetX() + GetWidth() - SLIDER_SIZE, GetY());
		m_pSliderVertical->SetSize(SLIDER_SIZE, GetHeight());
	}
	else if (!m_pSliderVertical->IsVisible() && m_pSliderHorizontal->IsVisible())
	{
		m_pSliderHorizontal->SetSize(GetWidth(), SLIDER_SIZE);
	}
	else if (m_pSliderVertical->IsVisible() && m_pSliderHorizontal->IsVisible())
	{
		m_pSliderVertical->SetPosition(GetX() + GetWidth() - SLIDER_SIZE, GetY() + SLIDER_SIZE);
		m_pSliderVertical->SetSize(SLIDER_SIZE, GetHeight() - SLIDER_SIZE);
		m_pSliderHorizontal->SetSize(GetWidth() - SLIDER_SIZE, SLIDER_SIZE);
	}

	if (!IsVisible())
	{
		m_pSliderVertical->SetVisible(false);
		m_pSliderHorizontal->SetVisible(false);
	}

	m_pSliderVertical->SetRatio(m_pSliderVertical->GetHeight() / (float)desc.Height);
	m_pSliderHorizontal->SetRatio(m_pSliderHorizontal->GetWidth() / (float)desc.Width);

	m_pSliderVertical->SetPercentage(0.0F);
	m_pSliderHorizontal->SetPercentage(0.0F);

	OnSliderChange(m_pSliderVertical, 0.0F);
	OnSliderChange(m_pSliderHorizontal, 0.0F);

	RequestRepaint();
}

float PanelScrollable::GetXInWorld(const GUIObject* child) const noexcept
{
	float value = GetX();
	if (child && IsMyChild(child))
	{
		value -= m_ClientOffset.x;
	}
	if (HasParent())
	{
		value += GetParent()->GetXInWorld(this);
	}
	return value;
}

float PanelScrollable::GetYInWorld(const GUIObject* child) const noexcept
{
	float value = GetY();
	if (child && IsMyChild(child))
	{
		value += m_ClientOffset.y;
	}
	if (HasParent())
	{
		value += GetParent()->GetYInWorld(this);
	}
	return value;
}

void PanelScrollable::OnAdded(GUIObject* parent)
{
	parent->Add(m_pSliderVertical);
	parent->Add(m_pSliderHorizontal);

	m_pSliderVertical->AddSliderListener(this);
	m_pSliderHorizontal->AddSliderListener(this);

	AddRealTimeRenderer();
	AddMouseListener(this);
}

void PanelScrollable::OnRemoved(GUIObject* parent)
{
	m_pSliderVertical->RemoveSliderListener(this);
	m_pSliderHorizontal->RemoveSliderListener(this);

	RemoveRealTimeRenderer();
	RemoveMouseListener(this);
}

void PanelScrollable::OnSliderChange(Slider* slider, float percentage)
{
	if (slider == m_pSliderVertical)
	{
		m_ClientOffset.y = (1.0 - slider->GetRatio()) * GetClientHeight() * (percentage - 1.0);
		if (m_pSliderHorizontal->IsVisible())
		{
			m_ClientOffset.y += SLIDER_SIZE;
		}
	}
	else if (slider == m_pSliderHorizontal)
	{
		m_ClientOffset.x = (1.0 - slider->GetRatio()) * GetClientWidth() * percentage;
	}

	glm::vec2 mousePos = Input::GetMousePosition();
	mousePos.y = Window::GetCurrentWindow().GetHeight() - mousePos.y;

	for (int i = s_MouseListeners.size() - 1; i >= 0; i--)
	{
		if (s_MouseListeners[i]->IsVisible() && IsMyChild(s_MouseListeners[i]))
		{
			s_MouseListeners[i]->OnMouseMove(mousePos);
		}
	}
}

bool PanelScrollable::ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept
{
	float x = GetXInWorld();
	float y = GetYInWorld();
	float heightIndent = m_pSliderHorizontal->IsVisible() * m_pSliderHorizontal->GetHeight();
	float widthIndent = m_pSliderVertical->IsVisible() * m_pSliderVertical->GetWidth();

	if (position.x > x && position.x < x + GetWidth() - widthIndent)
	{
		if (position.y > y + heightIndent && position.y < y + GetHeight())
		{
			if (HasParent())
			{
				return GetParent()->ContainsPoint(position);
			}
			return true;
		}
	}
	return false;
}

void PanelScrollable::RenderChildrensFrameBuffers(GUIContext* context)
{
	context->BeginSelfRendering(m_pFrameBufferClientArea, GetBackgroundColor());
	Panel::RenderChildrensFrameBuffers(context);
}

void PanelScrollable::RenderRealTimePre(GUIContext* context, float x, float y)
{
	float heightIndent = m_pSliderHorizontal->IsVisible() * m_pSliderHorizontal->GetHeight();
	float widthIndent = m_pSliderVertical->IsVisible() * m_pSliderVertical->GetWidth();
	glScissor(x, y + heightIndent, GetWidth() - widthIndent, GetHeight() - heightIndent);
	glEnable(GL_SCISSOR_TEST);
}

void PanelScrollable::RenderRealTime(GUIContext* context, float x, float y)
{
	context->RenderTexture((Texture2D*)m_pFrameBufferClientArea->GetColorAttachment(0), x - m_ClientOffset.x, y + m_ClientOffset.y, GetClientWidth(), GetClientHeight(), GUIContext::COLOR_WHITE);
}

void PanelScrollable::RenderRealTimePost(GUIContext* context)
{
	glm::vec4 viewPortSize = context->GetGraphicsContext()->GetViewPort();
	glScissor(viewPortSize.z, viewPortSize.w, viewPortSize.x, viewPortSize.y);
	glDisable(GL_SCISSOR_TEST);
}

void PanelScrollable::OnMouseScroll(const glm::vec2& position, const glm::vec2& offset)
{
	if (ContainsPoint(position, this))
	{
		if (m_pSliderVertical->IsVisible())
		{
			m_pSliderVertical->AccelerateSlider(offset.y * 600 * m_pSliderVertical->GetRatio());
		}
		else if (m_pSliderHorizontal->IsVisible())
		{
			m_pSliderHorizontal->AccelerateSlider(-offset.y * 600 * m_pSliderHorizontal->GetRatio());
		}
	}
}

void PanelScrollable::PrintName() const
{
	std::cout << "Panel Scrollable";
}