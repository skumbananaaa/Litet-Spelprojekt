#include <EnginePch.h>
#include <Graphics\GUI\PanelExpandable.h>

PanelExpandable::PanelExpandable(float x, float y, float width, float height, float clientHeight, const std::string& text) : Button(x, y, width, height, text),
	m_pFrameBufferClientArea(nullptr),
	m_Mode(CLOSED),
	m_Percentage(0),
	m_ClientAreaColor(0.408F, 0.408F, 0.408F, 1.0F)
{
	SetClientSize(clientHeight);
	SetBackgroundColor(glm::vec4(0.118F, 0.118F, 0.118F, 1.0F));
	SetOnPressedColor(glm::vec4(0.6F, 0.6F, 0.6F, 1.0F));
	SetOnHoverColor(glm::vec4(0.408F, 0.408F, 0.408F, 1.0F));
}

PanelExpandable::~PanelExpandable()
{
	DeleteSafe(m_pFrameBufferClientArea);
}

void PanelExpandable::SetSelected(bool selected)
{
	Button::SetSelected(selected);
	SetExpanded(selected);
}

void PanelExpandable::SetExpanded(bool expanded) noexcept
{
	if ((m_Mode == CLOSED || m_Mode == COLLAPSING) && expanded)
	{
		m_Mode = EXPANDING;
		AddRealTimeRenderer();
	}
	if ((m_Mode == OPEN || m_Mode == EXPANDING) && !expanded)
	{
		m_Mode = COLLAPSING;
	}
}

bool PanelExpandable::IsExpanded() const noexcept
{
	return m_Mode == OPEN || m_Mode == EXPANDING || m_Mode == COLLAPSING;
}

float PanelExpandable::GetClientWidth() const noexcept
{
	return m_pFrameBufferClientArea->GetWidth();
}

float PanelExpandable::GetClientHeight() const noexcept
{
	return m_pFrameBufferClientArea->GetHeight();
}

void PanelExpandable::AddExpandableListener(IExpandableListener* listener)
{
	if (!Contains<IExpandableListener>(m_Listeners, listener))
	{
		m_Listeners.push_back(listener);
	}
	else
	{
		std::cout << "ExpandableListener already added" << std::endl;
	}
}

void PanelExpandable::RemoveExpandableListener(IExpandableListener* listener)
{
	int32 counter = 0;
	for (IExpandableListener* object : m_Listeners)
	{
		if (object == listener)
		{
			m_Listeners.erase(m_Listeners.begin() + counter);
			return;
		}
		counter++;
	}
}

const glm::vec4& PanelExpandable::GetClientAreaColor() const noexcept
{
	return m_ClientAreaColor;
}

void PanelExpandable::SetClientAreaColor(const glm::vec4& color)
{
	if (m_ClientAreaColor != color)
	{
		m_ClientAreaColor = color;
		RequestRepaint();
	}
}

float PanelExpandable::GetPercentage() const noexcept
{
	return m_Percentage;
}

float PanelExpandable::GetYForClientArea() const noexcept
{
	return GetY() - GetClientHeight() * m_Percentage;
}

void PanelExpandable::SetClientSize(float height) noexcept
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
	desc.Width = static_cast<uint32>(GetWidth());
	desc.Height = static_cast<uint32>(height < GetHeight() ? GetHeight() : height);

	m_pFrameBufferClientArea = new Framebuffer(desc);

	RequestRepaint();
}

bool PanelExpandable::ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept
{
	if (caller == this || IsMyChild(caller))
	{
		if (IsExpanded())
		{
			float x = GetXInWorld();
			float y = GetYInWorld();

			if (position.x > x && position.x < x + GetWidth())
			{
				if (position.y > y - GetClientHeight() * m_Percentage && position.y < y + GetHeight())
				{
					if (HasParent())
					{
						return GetParent()->ContainsPoint(position);
					}
					return true;
				}
			}
		}
		else if (caller == this)
		{
			return Button::ContainsPoint(position, caller);
		}
		return false;
	}
	return Button::ContainsPoint(position, caller);
}

void PanelExpandable::RenderChildrensFrameBuffers(GUIContext* context)
{
	context->BeginSelfRendering(m_pFrameBufferClientArea, m_ClientAreaColor);
	Button::RenderChildrensFrameBuffers(context);
}

void PanelExpandable::RenderRealTimePre(GUIContext* context, float x, float y)
{
	glScissor(x, y - GetClientHeight() * m_Percentage, GetWidth(), GetClientHeight() * m_Percentage + 1);
	glEnable(GL_SCISSOR_TEST);
}

void PanelExpandable::RenderRealTime(GUIContext* context, float x, float y)
{
	context->RenderTexture((Texture2D*)m_pFrameBufferClientArea->GetColorAttachment(0), x, y - GetClientHeight(), GetClientWidth(), GetClientHeight(), GUIContext::COLOR_WHITE);
}

void PanelExpandable::RenderRealTimePost(GUIContext* context)
{
	glm::vec4 viewPortSize = context->GetGraphicsContext()->GetViewPort();
	glScissor(viewPortSize.z, viewPortSize.w, viewPortSize.x, viewPortSize.y);
	glDisable(GL_SCISSOR_TEST);
}

float PanelExpandable::GetYInWorld(const GUIObject* child) const noexcept
{
	float value = GetY();
	if (child && IsMyChild(child))
	{
		value -= GetClientHeight();
	}
	if (HasParent())
	{
		value += GetParent()->GetYInWorld(this);
	}
	return value;
}

void PanelExpandable::OnUpdate(float dtS)
{
	static const float speed = 6.0F;

	if (m_Mode == EXPANDING)
	{
		m_Percentage += dtS * speed;
		if (m_Percentage > 1.0)
		{
			m_Percentage = 1.0;
			m_Mode = OPEN;
		}
		for (IExpandableListener* listener : m_Listeners)
		{
			listener->OnExpanding(this, m_Percentage);
		}
	}
	else if (m_Mode == COLLAPSING)
	{
		m_Percentage -= dtS * speed;
		if (m_Percentage < 0.0)
		{
			m_Percentage = 0.0;
			m_Mode = CLOSED;
			RemoveRealTimeRenderer();
		}
		for (IExpandableListener* listener : m_Listeners)
		{
			listener->OnCollapsing(this, m_Percentage);
		}
	}
}

void PanelExpandable::PrintName() const
{
	std::cout << "PanelExpandable" << std::endl;
}
