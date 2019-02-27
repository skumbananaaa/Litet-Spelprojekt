#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/GUIManager.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

std::vector<GUIObject*> GUIObject::s_MouseListeners;
std::vector<GUIObject*> GUIObject::s_KeyboardListeners;
Texture2D* GUIObject::s_pDefaultTexture = nullptr;

GUIObject::GUIObject(float x, float y, float width, float height) :
	m_Position(x, y),
	m_pFramebuffer(nullptr),
	m_pBackgroundTexture(nullptr),
	m_pParent(nullptr),
	m_IsVisible(true),
	m_pUserData(nullptr),
	m_IsDirty(false),
	m_IsRealtime(false),
	m_BackgroundColor(1.0, 1.0, 1.0, 1.0)
{
	if (width > 0 && height > 0)
	{
		RecreateFrameBuffer(width, height);
	}
}

GUIObject::~GUIObject()
{
	if (m_pFramebuffer)
	{
		delete m_pFramebuffer;
	}

	RemoveMouseListener(this);
	RemoveKeyboardListener(this);
	RemoveRealTimeRenderer();

	if (m_DeleteAll)
	{
		for (GUIObject* object : m_Children)
		{
			object->SetDeleteAllChildrenOnDestruction(true);
			delete object;
		}
	}
	m_Children.clear();
	m_ChildrenToAdd.clear();
	m_ChildrenToRemove.clear();
	m_ChildrenDirty.clear();
}

bool GUIObject::HasParent() const noexcept
{
	return m_pParent != nullptr;
}

GUIObject* GUIObject::GetParent() const noexcept
{
	return m_pParent;
}

void GUIObject::Add(GUIObject* object) noexcept
{
	if (!Contains<GUIObject>(m_Children, object) && !Contains<GUIObject>(m_ChildrenToAdd, object))
	{
		m_ChildrenToAdd.push_back(object);
	}
}

void GUIObject::Remove(GUIObject* object) noexcept
{
	if (Contains<GUIObject>(m_Children, object))
	{
		m_ChildrenToRemove.push_back(object);
		object->RemoveRealTimeRenderer();
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
			m_IsDirty = true;
		}
	}
}

const std::vector<GUIObject*>& GUIObject::GetChildrenToAdd() noexcept
{
	return m_ChildrenToAdd;
}

const std::vector<GUIObject*>& GUIObject::GetChildrenToRemove() noexcept
{
	return m_ChildrenToRemove;
}

void GUIObject::AddMouseListener(GUIObject* listener)
{
	if (!Contains<GUIObject>(s_MouseListeners, listener))
	{
		s_MouseListeners.push_back(listener);
	}
	else
	{
		std::cout << "MouseListener already added" << std::endl;
	}
}

void GUIObject::RemoveMouseListener(GUIObject* listener)
{
	int32 counter = 0;
	for (GUIObject* object : s_MouseListeners)
	{
		if (object == listener)
		{
			s_MouseListeners.erase(s_MouseListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void GUIObject::AddKeyboardListener(GUIObject* listener)
{
	if (!Contains<GUIObject>(s_KeyboardListeners, listener))
	{
		s_KeyboardListeners.push_back(listener);
	}
	else
	{
		std::cout << "CharFromKeyListener already added" << std::endl;
	}
}

void GUIObject::RemoveKeyboardListener(GUIObject* listener)
{
	int32 counter = 0;
	for (GUIObject* object : s_KeyboardListeners)
	{
		if (object == listener)
		{
			s_KeyboardListeners.erase(s_KeyboardListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void GUIObject::AddRealTimeRenderer()
{
	m_IsRealtime = true;
}

void GUIObject::RemoveRealTimeRenderer()
{
	m_IsRealtime = false;
}

bool GUIObject::IsDirty() const noexcept
{
	return m_IsDirty;
}

bool GUIObject::IsMyChild(const GUIObject* child) const noexcept
{
	return Contains<GUIObject>(m_Children, child);
}

Texture2D* GUIObject::GetBackgroundTexture() const noexcept
{
	return m_pBackgroundTexture;
}

void GUIObject::SetBackgroundTexture(Texture2D* texture)
{
	if (m_pBackgroundTexture != texture)
	{
		m_pBackgroundTexture = GetDefaultTexture();
		RequestRepaint();
	}
}

const glm::vec4& GUIObject::GetBackgroundColor() const noexcept
{
	return m_BackgroundColor;
}

void GUIObject::SetBackgroundColor(const glm::vec4& color) noexcept
{
	if (m_BackgroundColor != color)
	{
		m_BackgroundColor = color;
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

float GUIObject::GetXInWorld(const GUIObject* child) const noexcept
{
	float value = GetX();
	if (HasParent())
	{
		value += GetParent()->GetXInWorld(this);
	}
	return value;
}

float GUIObject::GetYInWorld(const GUIObject* child) const noexcept
{
	float value = GetY();
	if (HasParent())
	{
		value += GetParent()->GetYInWorld(this);
	}
	return value;
}

void GUIObject::SetSize(float width, float height) noexcept
{
	if (GetWidth() != width || GetHeight() != height)
	{
		RecreateFrameBuffer(width, height);
		RequestRepaint();
	}
}

void GUIObject::SetPosition(float x, float y) noexcept
{
	if (m_Position.x != x || m_Position.y != y)
	{
		m_Position.x = x;
		m_Position.y = y;
		if (HasParent())
		{
			GetParent()->RequestRepaint();
		}
	}
}

void GUIObject::SetVisible(bool visible) noexcept
{
	if (m_IsVisible != visible)
	{
		m_IsVisible = visible;
		RequestRepaint();
	}
}

bool GUIObject::IsVisible() noexcept
{
	if (HasParent())
	{
		return m_IsVisible && GetParent()->IsVisible();
	}
	return m_IsVisible;
}

bool GUIObject::IsRealtimeRendered() const noexcept
{
	return m_IsRealtime;
}

void GUIObject::InternalOnUpdate(float dtS)
{
	/*
	* Remove the children who wants to be removed
	*/
	if (!m_ChildrenToRemove.empty())
	{
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
					return;
				}
				counter++;
			}
		}
		m_ChildrenToRemove.clear();
		RequestRepaint();
	}
	

	/*
	* Add the children who wants to be added
	*/
	if (!m_ChildrenToAdd.empty())
	{
		static std::vector<GUIObject*> newChildren;

		for (GUIObject* objectToAdd : m_ChildrenToAdd)
		{
			newChildren.push_back(objectToAdd);
		}
		m_ChildrenToAdd.clear();
		
		for (GUIObject* objectToAdd : newChildren)
		{
			m_Children.push_back(objectToAdd);
			objectToAdd->m_pParent = this;
			objectToAdd->RequestRepaint();
			objectToAdd->OnAdded(this);
		}
		newChildren.clear();
	}

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
	this->OnPreRender(context);
	context->BeginSelfRendering(m_pFramebuffer, GetClearColor());
	this->OnRender(context);
	for (IExternalUIRenderer* object : m_ExternalRenderers)
	{
		object->OnRenderGUIObject(context, this);
	}

	/*
	* Render all childrens Framebuffers
	*/
	RenderChildrensFrameBuffers(context);

	m_IsDirty = false;
}

void GUIObject::InternalOnRealtimeRender(GUIContext* context)
{
	float x = GetXInWorld();
	float y = GetYInWorld();
	RenderRealTimePre(context, x, y);
	if (IsRealtimeRendered())
	{
		RenderRealTime(context, x, y);
	}
	for (GUIObject* object : m_Children)
	{
		if (object->IsVisible())
		{
			object->InternalOnRealtimeRender(context);
		}
	}
	RenderRealTimePost(context);
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

	InternalOnRealtimeRender(context);
}

void GUIObject::RerenderChildren(GUIContext* context)
{
	for (GUIObject* child : m_ChildrenDirty)
	{
		child->InternalOnRender(context);
	}
	m_ChildrenDirty.clear();
}

void GUIObject::RecreateFrameBuffer(float width, float height)
{
	if (m_pFramebuffer)
	{
		if (GetWidth() == width && GetHeight() == height)
		{
			return;
		}
		delete m_pFramebuffer;
	}

	FramebufferDesc desc;
	desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
	desc.SamplingParams = { TEX_PARAM_EDGECLAMP, TEX_PARAM_LINEAR, TEX_PARAM_LINEAR };
	desc.NumColorAttachments = 1;
	desc.Width = static_cast<uint32>(width);
	desc.Height = static_cast<uint32>(height);

	m_pFramebuffer = new Framebuffer(desc);
}

void GUIObject::CreateDefaultTexture()
{
	if (!s_pDefaultTexture)
	{
		TextureDesc desc;
		desc.Format = TEX_FORMAT_RGBA;
		desc.Width = 1;
		desc.Height = 1;
		desc.Samples = 0;
		desc.GenerateMips = false;

		uint8 pixel[] = { 255, 255, 255, 255 };

		s_pDefaultTexture = new Texture2D(pixel, desc);
	}
}

void GUIObject::DeleteDefaultTexture()
{
	if (s_pDefaultTexture)
	{
		delete s_pDefaultTexture;
	}
}

void GUIObject::RenderChildrensFrameBuffers(GUIContext* context)
{
	for (GUIObject* child : m_Children)
	{
		if (child->m_IsVisible)
		{
			context->RenderFrameBuffer(child->m_pFramebuffer, child->GetX(), child->GetY());
		}
	}
}

void GUIObject::RenderRealTime(GUIContext* context, float x, float y)
{
}

void GUIObject::OnRender(GUIContext* context)
{
	RenderBackgroundTexture(context);
}

void GUIObject::RenderBackgroundTexture(GUIContext* context)
{
	Texture2D* texture = GetClearTexture();
	if (texture)
	{
		context->RenderTexture(texture, 0, 0, GetWidth(), GetHeight(), GUIContext::COLOR_WHITE);
	}
}

bool GUIObject::ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept
{
	float x = GetXInWorld();
	float y = GetYInWorld();

	if (position.x > x && position.x < x + GetWidth())
	{
		if (position.y > y && position.y < y + GetHeight())
		{
			if (HasParent())
			{
				return GetParent()->ContainsPoint(position, caller);
			}
			return true;
		}
	}
	return false;
}

bool GUIObject::ContainsPoint(const glm::vec2& position) const noexcept
{
	return ContainsPoint(position, this);
}

bool GUIObject::OwnsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept
{
	return ContainsPoint(position, caller);
}

bool GUIObject::OwnsPoint(const glm::vec2& position) const noexcept
{
	return OwnsPoint(position, this);
}

void GUIObject::DeleteChildren()
{
	for (GUIObject* object : m_Children)
	{
		object->DeleteChildren();
		delete object;
	}
	m_Children.clear();
}

void GUIObject::SetDeleteAllChildrenOnDestruction(bool deleteAll)
{
	m_DeleteAll = deleteAll;
}

bool GUIObject::WillDeleteAllChildrenOnDestruction() const noexcept
{
	return m_DeleteAll;
}

void GUIObject::AddExternalRenderer(IExternalUIRenderer* renderer)
{
	if (!Contains<IExternalUIRenderer>(m_ExternalRenderers, renderer))
	{
		m_ExternalRenderers.push_back(renderer);
	}
	else
	{
		std::cout << "IExternalUIRenderer already added" << std::endl;
	}
}

void GUIObject::RemoveExternalRenderer(IExternalUIRenderer* renderer)
{
	int32 counter = 0;
	for (IExternalUIRenderer* object : m_ExternalRenderers)
	{
		if (object == renderer)
		{
			m_ExternalRenderers.erase(m_ExternalRenderers.begin() + counter);
			return;
		}
		counter++;
	}
}

void GUIObject::SetUserData(void* data)
{
	m_pUserData = data;
}

void* GUIObject::GetUserData() const
{
	return m_pUserData;
}

const std::vector<GUIObject*>& GUIObject::GetChildren() noexcept
{
	return m_Children;
}

int32 GUIObject::GetNrOfChildren() const noexcept
{
	return m_Children.size() + m_ChildrenToAdd.size() - m_ChildrenToRemove.size();
}

Texture2D* GUIObject::GetDefaultTexture() const
{
	return s_pDefaultTexture;
}

const glm::vec4& GUIObject::GetClearColor() const
{
	return GetBackgroundColor();
}

Texture2D* GUIObject::GetClearTexture() const
{
	return GetBackgroundTexture();
}

void GUIObject::InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	for (int i = s_MouseListeners.size() - 1; i >= 0; i--)
	{
		if (s_MouseListeners[i]->IsVisible())
		{
			s_MouseListeners[i]->OnMousePressed(position, mousebutton);
		}
	}
}

void GUIObject::InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	for (int i = s_MouseListeners.size() - 1; i >= 0; i--)
	{
		if (s_MouseListeners[i]->IsVisible())
		{
			s_MouseListeners[i]->OnMouseReleased(position, mousebutton);
		}
	}
}

void GUIObject::InternalRootOnMouseMove(const glm::vec2& position)
{
	for (int i = s_MouseListeners.size() - 1; i >= 0; i--)
	{
		if (s_MouseListeners[i]->IsVisible())
		{
			s_MouseListeners[i]->OnMouseMove(position);
		}
	}
}

void GUIObject::InternalRootOnMouseScroll(const glm::vec2& position, const glm::vec2& offset)
{
	for (int i = s_MouseListeners.size() - 1; i >= 0; i--)
	{
		if (s_MouseListeners[i]->IsVisible())
		{
			s_MouseListeners[i]->OnMouseScroll(position, offset);
		}
	}
	InternalRootOnMouseMove(position);
}

void GUIObject::InternalRootOnKeyUp(KEY keycode)
{
	for (int i = s_KeyboardListeners.size() - 1; i >= 0; i--)
	{
		if (s_KeyboardListeners[i]->IsVisible())
		{
			s_KeyboardListeners[i]->OnKeyUp(keycode);
		}
	}
}

void GUIObject::InternalRootOnKeyDown(KEY keycode)
{
	for (int i = s_KeyboardListeners.size() - 1; i >= 0; i--)
	{
		if (s_KeyboardListeners[i]->IsVisible())
		{
			s_KeyboardListeners[i]->OnKeyDown(keycode);
		}
	}
}

void GUIObject::InternalRootOnCharFromKey(char c)
{
	for (int i = s_KeyboardListeners.size() - 1; i >= 0; i--)
	{
		if (s_KeyboardListeners[i]->IsVisible())
		{
			s_KeyboardListeners[i]->OnCharFromKey(c);
		}
	}
}
