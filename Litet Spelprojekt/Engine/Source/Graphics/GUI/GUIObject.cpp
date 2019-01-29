#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/GUIManager.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

std::vector<IMouseListener*> GUIObject::m_MouseListeners;

GUIObject::GUIObject(float x, float y, float width, float height) :
	m_Position(x, y),
	m_pFramebuffer(nullptr),
	m_pBackgroundTexture(nullptr)
{
	if (width > 0 && height > 0)
	{
		FramebufferDesc desc;
		desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.SamplingParams = TextureParams();
		desc.NumColorAttachments = 1;
		desc.Width = static_cast<uint32>(width);
		desc.Height = static_cast<uint32>(height);

		m_pFramebuffer = new Framebuffer(desc);
	}
}

GUIObject::~GUIObject()
{
	if (m_pFramebuffer)
	{
		delete m_pFramebuffer;
	}
}

bool GUIObject::HasParent() const
{
	return m_pParent != nullptr;
}

GUIObject* GUIObject::GetParent() const
{
	return m_pParent;
}

void GUIObject::Add(GUIObject* object)
{
	if (!Contains<GUIObject>(m_Children, object) && !Contains<GUIObject>(m_ChildrenToAdd, object))
	{
		m_ChildrenToAdd.push_back(object);
	}
}

void GUIObject::Remove(GUIObject* object)
{
	if (Contains<GUIObject>(m_Children, object))
	{
		m_ChildrenToRemove.push_back(object);
	}
}

void GUIObject::RequestRepaint()
{
	if (HasParent())
	{
		if (!m_IsDirty)
		{
			GetParent()->m_ChildrenDirty.push_back(this);
			GetParent()->RequestRepaint();
		}
	}
	m_IsDirty = true;
}

void GUIObject::AddMouseListener(IMouseListener* listener)
{
	if (!Contains<IMouseListener>(m_MouseListeners, listener))
	{
		m_MouseListeners.push_back(listener);
	}
}

void GUIObject::RemoveMouseListener(IMouseListener* listener)
{
	int32 counter = 0;
	for (IMouseListener* object : m_MouseListeners)
	{
		if (object == listener)
		{
			m_MouseListeners.erase(m_MouseListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

bool GUIObject::IsDirty() const noexcept
{
	return m_IsDirty;
}

Texture2D* GUIObject::GetTexture() const noexcept
{
	return m_pBackgroundTexture;
}

void GUIObject::SetTexture(Texture2D* texture)
{
	if (m_pBackgroundTexture != texture)
	{
		m_pBackgroundTexture = texture;
		RequestRepaint();
	}
}

float GUIObject::GetWidth() const noexcept
{
	return m_pFramebuffer->GetWidth();
}

float GUIObject::GetHeight() const noexcept
{
	return m_pFramebuffer->GetHeight();
}

float GUIObject::GetX() const noexcept
{
	return m_Position.x;
}

float GUIObject::GetY() const noexcept
{
	return m_Position.y;
}

float GUIObject::GetXInWorld() const noexcept
{
	float value = GetX();
	if (HasParent())
	{
		value += GetParent()->GetXInWorld();
	}
	return value;
}

float GUIObject::GetYInWorld() const noexcept
{
	float value = GetY();
	if (HasParent())
	{
		value += GetParent()->GetYInWorld();
	}
	return value;
}

void GUIObject::InternalOnUpdate(float dtS)
{
	/*
	* Remove the children who wants to be removed
	*/
	for (GUIObject* objectToRemove : m_ChildrenToRemove)
	{
		int32 counter = 0;
		for (GUIObject* object : m_Children)
		{
			if (objectToRemove == object)
			{
				m_Children.erase(m_Children.begin() + counter);
				objectToRemove->OnRemoved(this);
				objectToRemove->m_pParent = nullptr;
				std::cout << "GUI Object Removed" << std::endl;
				return;
			}
			counter++;
		}
	}

	/*
	* Add the children who wants to be added
	*/
	for (GUIObject* objectToAdd : m_ChildrenToAdd)
	{
		m_Children.push_back(objectToAdd);
		m_ChildrenDirty.push_back(objectToAdd);
		objectToAdd->m_pParent = this;
		objectToAdd->OnAdded(this);
		std::cout << "GUI Object Added" << std::endl;
	}
	m_ChildrenToRemove.clear();
	m_ChildrenToAdd.clear();

	/*
	* Let the children do the same checks as above
	*/
	for (GUIObject* object : m_Children)
	{
		object->InternalOnUpdate(dtS);
	}

	/*
	* Finally update my self and do what ever I want to do
	*/
	this->OnUpdate(dtS);
}


void GUIObject::InternalOnRender(GUIContext* context)
{
	/*
	* Render all children that needs repaint
	*/
	RerenderChildren(context);

	/*
	* Render my self
	*/
	context->BeginSelfRendering(m_pFramebuffer);
	this->OnRender(context);

	/*
	* Render all childrens Framebuffers
	*/
	RenderChildrensFrameBuffers(context);

	m_IsDirty = false;
}

/*
* Render tree, called from GUIManager
*/
void GUIObject::InternalRootOnRender(GUIContext* context)
{
	/*
	* Render all children that needs repaint
	*/
	RerenderChildren(context);

	/*
	* Render all childrens Framebuffers
	*/
	context->BeginRootRendering();
	RenderChildrensFrameBuffers(context);
}

void GUIObject::RerenderChildren(GUIContext* context)
{
	for (GUIObject* child : m_ChildrenDirty)
	{
		child->InternalOnRender(context);
	}
	m_ChildrenDirty.clear();
}

void GUIObject::RenderChildrensFrameBuffers(GUIContext* context)
{
	for (GUIObject* child : m_Children)
	{
		context->RenderFrameBuffer(child->m_pFramebuffer, child->GetX(), child->GetY());
	}
}

void GUIObject::OnRender(GUIContext* context)
{
	RenderBackgroundTexture(context);
}

void GUIObject::RenderBackgroundTexture(GUIContext* context)
{
	context->SetVertexQuadData(0, 0, GetWidth(), GetHeight());
	if (m_pBackgroundTexture)
	{
		context->GetGraphicsContext()->SetTexture(m_pBackgroundTexture, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		context->GetGraphicsContext()->SetTexture(nullptr, 0);
	}
}

bool GUIObject::ContainsPoint(const glm::vec2& position)
{
	float x = GetXInWorld();
	float y = GetYInWorld();

	if (position.x > x && position.x < x + GetWidth())
	{
		if (position.y > y && position.y < y + GetHeight())
		{
			return true;
		}
	}
	return false;
}

void GUIObject::InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	for (IMouseListener* listener : m_MouseListeners)
	{
		listener->OnMousePressed(position, mousebutton);
	}
}

void GUIObject::InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	for (IMouseListener* listener : m_MouseListeners)
	{
		listener->OnMouseReleased(position, mousebutton);
	}
}

void GUIObject::InternalRootOnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	for (IMouseListener* listener : m_MouseListeners)
	{
		listener->OnMouseMove(lastPosition, position);
	}
}