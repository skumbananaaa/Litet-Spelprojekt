#include <EnginePch.h>
#include <World/Scenarios/Water/WaterObject.h>

WaterObject::WaterObject() noexcept : GameObject()
{
}

WaterObject::~WaterObject()
{
}

void WaterObject::Update(float deltaTime) noexcept
{
	Extend(deltaTime);

	if (IsExtended())
	{
		UpdateTransform();
	}
}
