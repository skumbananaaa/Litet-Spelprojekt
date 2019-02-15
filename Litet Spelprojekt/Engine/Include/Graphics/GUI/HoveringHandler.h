#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/IHoverable.h>

class HoveringHandler;

class API IHoveringListener
{
public:
	virtual void OnHovered(const HoveringHandler* handler, IHoverable* selection) = 0;
	virtual void OnDehovered(const HoveringHandler* handler, IHoverable* selection) = 0;
};

class API HoveringHandler : public IHoverableListener
{
public:
	HoveringHandler();
	~HoveringHandler();

	void Release();

	void AddHoverable(IHoverable* selectable);
	void RemoveHoverable(IHoverable* selectable);
	bool AddHoveringListener(IHoveringListener* listener);
	void RemoveHoveringListener(IHoveringListener* listener);

	IHoverable* GetHovered();

	virtual void SendUpdate(IHoverable* selection) override;

private:
	virtual void TriggerOnHovered(IHoverable* selection) const;
	virtual void TriggerOnDehovered(IHoverable* selection) const;

private:
	std::vector<IHoverable*> m_Hoverables;
	std::vector<IHoveringListener*> m_HoveringListeners;
	int32 m_UpdateCounter;
	IHoverable* m_LastHoverable;
};