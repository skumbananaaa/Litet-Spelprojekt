#include <EnginePch.h>
#include <Graphics/GUI/PanelScrollable.h>

PanelScrollable::PanelScrollable(float x, float y, float width, float height, float clientWidth, float clientHeight, Texture2D* texture, Texture2D* sliderBackground, Texture2D* sliderForeground) : Panel(x, y, width, height, texture),
	m_pFrameBufferClientArea(nullptr),
	m_ClientOffset(0, 0)
{
	static float size = 20;
	m_pSliderVertical = new Slider(x + width - size, y + size, size, height - size, sliderBackground, sliderForeground);
	m_pSliderHorizontal = new Slider(x, y, width - size, size, sliderBackground, sliderForeground);
	SetClientSize(clientWidth, clientHeight);
}

PanelScrollable::~PanelScrollable()
{
	delete m_pSliderVertical;
	delete m_pSliderHorizontal;
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

	m_pSliderVertical->SetRatio(GetHeight() / (float)desc.Height);
	m_pSliderHorizontal->SetRatio(GetWidth() / (float)desc.Width);

	m_pSliderVertical->SetPercentage(0.0F);
	m_pSliderHorizontal->SetPercentage(0.0F);

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
	m_pSliderVertical->AddSliderListener(this);
	m_pSliderHorizontal->AddSliderListener(this);

	parent->Add(m_pSliderVertical);
	parent->Add(m_pSliderHorizontal);

	AddRealTimeRenderer(this);
}

void PanelScrollable::OnRemoved(GUIObject* parent)
{
	m_pSliderVertical->RemoveSliderListener(this);
	m_pSliderHorizontal->RemoveSliderListener(this);

	parent->Remove(m_pSliderVertical);
	parent->Remove(m_pSliderHorizontal);

	RemoveRealTimeRenderer(this);
}

void PanelScrollable::OnSliderChange(Slider* slider, float percentage)
{
	if (slider == m_pSliderVertical)
	{
		m_ClientOffset.y = slider->GetRatio() * GetClientHeight() * percentage;
	}
	else if (slider == m_pSliderHorizontal)
	{
		m_ClientOffset.x = slider->GetRatio() * GetClientWidth() * percentage;
	}
}

void PanelScrollable::RenderChildrensFrameBuffers(GUIContext* context)
{
	context->BeginSelfRendering(m_pFrameBufferClientArea);
	Panel::RenderChildrensFrameBuffers(context);
}

void PanelScrollable::RenderRealTime(GUIContext* context)
{
	context->SetVertexQuadData(GetXInWorld() - m_ClientOffset.x, GetYInWorld() + m_ClientOffset.y, GetClientWidth(), GetClientHeight());
	context->GetGraphicsContext()->SetTexture(m_pFrameBufferClientArea->GetColorAttachment(0), 0);
	glm::vec4 viewPortSize = context->GetGraphicsContext()->GetViewPort();
	glScissor(GetXInWorld(), GetYInWorld(), GetWidth(), GetHeight());
	glEnable(GL_SCISSOR_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glScissor(viewPortSize.z, viewPortSize.w, viewPortSize.x, viewPortSize.y);
	glDisable(GL_SCISSOR_TEST);
	context->GetGraphicsContext()->SetTexture(nullptr, 0);
}