#pragma once
#include "IScenario.h"
#include <World/GameObjectDoor.h>

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

constexpr float WATER_EVAPORATION_BY_FIRE_RATE = 1.0f / 10000.0f;

class ScenarioFire : public IScenario
{
public:
	ScenarioFire(bool fireAlwaysVisible = false);
	virtual ~ScenarioFire();

	virtual void Init(World* pWorld) noexcept override;
	virtual void OnStart(SceneGame* scene) noexcept override;
	virtual void OnEnd(SceneGame* scene)noexcept override;
	virtual void Escalate(const glm::ivec3& position) noexcept override;
	virtual void OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept override;
	virtual bool Update(float dtS, World* world, SceneGame* scene) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

	void ShowInRoom(uint32 roomID) noexcept;

private:
	World* m_pWorld;
	bool m_FireAlwaysVisible;

	const uint32* const** m_pppMap;

	void SpreadFireSideways(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene);
	bool SpreadSmokeSideways(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* scene);
	float CalculateDoorSpreadFactor(const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, bool spreadingThroughBulkhead, float rateOfNormalDoorSpread, float rateOfBulkheadDoorSpreadFactor, float rateOfBulkheadSpreadFactor) const noexcept;
	float CalculateBulkheadSpreadFactor(bool spreadingThroughBulkhead, float rateOfBulkheadSpreadFactor) const noexcept;

	void EvaporateWater(TileData& tile, float dtS) const noexcept;
	void SetFireVisible(uint32 roomId, bool show = true) noexcept;
	void SetSmokeVisible(uint32 roomId, bool show = true) noexcept;

	// x, y, z = x, level, z
	std::vector<glm::ivec3> m_OnFire;
	std::vector<glm::ivec3> m_Smoke;
	std::vector<bool> m_DiscoveredRooms;
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

inline void ScenarioFire::EvaporateWater(TileData& tile, float dtS) const noexcept
{
	if (tile.AlreadyFlooded)
	{
		tile.WaterLevelChange -= tile.Temp * WATER_EVAPORATION_BY_FIRE_RATE * dtS;
	}
}

inline void ScenarioFire::SetFireVisible(uint32 roomId, bool show) noexcept
{
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3 pos = m_OnFire[i];
		uint32 tileID = m_pWorld->GetLevel(pos.y).GetLevel()[pos.x][pos.z];
		if (roomId == tileID)
		{
			GameObject* pObject = m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_FIRE];
			if (pObject)
			{
				pObject->SetIsVisible(show);
			}
		}
	}
}

inline void ScenarioFire::SetSmokeVisible(uint32 roomId, bool show) noexcept
{
	for (uint32 i = 0; i < m_Smoke.size(); i++)
	{
		glm::ivec3 pos = m_Smoke[i];
		
		if (roomId == m_pWorld->GetLevel(pos.y).GetLevel()[pos.x][pos.z])
		{
			GameObject* pObject = m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE];
			if (pObject)
			{
				pObject->SetIsVisible(show);
			}
		}
	}
}
