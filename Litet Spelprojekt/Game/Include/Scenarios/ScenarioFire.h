#pragma once
#include "IScenario.h"
#include "../GameObjectDoor.h"

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

constexpr float WATER_EVAPORATION_BY_FIRE_RATE = 1.0f / 10000.0f;
constexpr float SMOKE_DISSOLVE_RATE = 10.0f;

class ScenarioFire : public IScenario
{
public:
	ScenarioFire(bool fireAlwaysVisible = false);
	virtual ~ScenarioFire();

	virtual void Init(World* pWorld) noexcept override;
	virtual void Release() noexcept override;
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
