#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/GUIManager.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

GUIObject::GUIObject(float x, float y, float width, float height, std::string name) :
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
	m_Name = name;
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
	if (!Contains(m_Children, object) && !Contains(m_ChildrenToAdd, object))
	{
		m_ChildrenToAdd.push_back(object);
	}
}

void GUIObject::Remove(GUIObject* object)
{
	if (Contains(m_Children, object))
	{
		m_ChildrenToRemove.push_back(object);
	}
}

bool GUIObject::Contains(const std::vector<GUIObject*>& list, GUIObject* object)
{
	for (GUIObject* parent : list)
	{
		if (parent == object)
		{
			return true;
		}
	}
	return false;
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
	m_pBackgroundTexture = texture;
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

void GUIObject::OnAdded(GUIObject* parent)
{
	
}

void GUIObject::OnRemoved(GUIObject* parent)
{
	
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

void GUIObject::OnUpdate(float dtS)
{
	
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
	context->SetVertexQuadData(0, 0, GetWidth(), GetHeight());
	if (m_pBackgroundTexture)
	{
		context->GetGraphicsContext()->SetTexture(m_pBackgroundTexture, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		context->GetGraphicsContext()->SetTexture(nullptr, 0);
	}
}

void GUIObject::OnMousePressed(MouseButton mousebutton)
{

}

void GUIObject::OnMouseReleased(MouseButton mousebutton)
{

}

void GUIObject::OnMouseMove(const glm::vec2& position)
{

}

void GUIObject::OnKeyUp(KEY keycode)
{

}

void GUIObject::OnKeyDown(KEY keycode)
{

}