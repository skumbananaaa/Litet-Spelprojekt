#include <EnginePch.h>
#include <Graphics/Scenarios/WaterObject.h>

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
