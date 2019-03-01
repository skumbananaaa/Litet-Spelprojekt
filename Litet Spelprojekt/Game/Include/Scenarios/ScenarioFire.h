#pragma once
#include <World/Scenarios/IScenario.h>
#include <World/GameObjectDoor.h>

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

class ScenarioFire : public IScenario
{
public:
	ScenarioFire();
	virtual ~ScenarioFire();

	virtual void Init(World* pWorld) noexcept override;
	virtual void OnStart(Scene* scene) noexcept override;
	virtual void OnEnd(Scene* scene)noexcept override;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual bool Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

	void ShowInRoom(uint32 roomID) noexcept;
private:
	World* m_pWorld;

	const uint32* const** m_pppMap;

	void CheckFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene);
	bool CheckSmoke(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* scene);
	float CalculateDoorSpreadFactor(const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, bool spreadingThroughBulkhead, float rateOfNormalDoorSpread, float rateOfBulkheadDoorSpreadFactor, float rateOfBulkheadSpreadFactor) const noexcept;
	float CalculateBulkheadSpreadFactor(bool spreadingThroughBulkhead, float rateOfBulkheadSpreadFactor) const noexcept;

	// x, y, z = x, level, z
	std::vector<glm::ivec3> m_OnFire;
	std::vector<glm::ivec3> m_Smoke;
};

inline float ScenarioFire::CalculateDoorSpreadFactor(
	const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo,
	bool spreadingThroughBulkhead, float rateOfNormalDoorSpread, float rateOfBulkheadDoorSpreadFactor, float rateOfBulkheadSpreadFactor) const noexcept
{
	if (ppLevelData[tileFrom.x][tileFrom.y].HasDoor() && ppLevelData[tileTo.x][tileTo.y].HasDoor())
	{
		bool doorIsOpen = !reinterpret_cast<GameObjectDoor*>(ppLevelData[tileTo.x][tileTo.y].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR])->IsClosed();

		//If the smallest of the tiles x coordinates is a multiple of "tilesBetweenBulkheads", the water is trying to flood over a bulkhead.
		//Since CanSpreadTo returns 0 if there isnt a door when the water is trying to flood to a different room we know its trying to flood over a door.
		if (spreadingThroughBulkhead)
		{
			return doorIsOpen ? 1.0f / rateOfBulkheadSpreadFactor : rateOfBulkheadDoorSpreadFactor;
		}

		//If the water is trying to flood over a door that is not in a bulkhead, reduce the flood factor.
		return doorIsOpen ? 1.0f : rateOfNormalDoorSpread;
	}

	//Water is not trying to flood to a different room but tileTo has a door.
	return 1.0f;
}

inline float ScenarioFire::CalculateBulkheadSpreadFactor(bool spreadingThroughBulkhead, float rateOfBulkheadSpreadFactor) const noexcept
{
	return spreadingThroughBulkhead ? rateOfBulkheadSpreadFactor : 1.0f;
}