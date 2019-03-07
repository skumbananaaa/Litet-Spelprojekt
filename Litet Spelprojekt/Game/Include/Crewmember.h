#pragma once

#include <Graphics/GameObject.h>
#include "../Include/Orders/OrderHandler.h"
#include "../Include/GUI/UISelectedCrew.h"

#pragma message("INCLUDE" __FILE__)

#define CHOSEN_LIGHT glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_LIGHT glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)
#define MAX_NR_OF_ORDERS 10

#define CREWMEMBER_FULL_HEALTH_MOVEMENT_SPEED 2.5f
#define CREWMEMBER_LIGHTLY_INJURED_MOVEMENT_SPEED 1.8f
#define CREWMEMBER_SERIOUSLY_INJURED_MOVEMENT_SPEED 0.8f
#define CREWMEMBER_DEAD_MOVEMENT_SPEED 0.0f

#define CREWMEMBER_IDLE_MOVEMENT_SPEED_MULTIPLIER 0.7f

#define NR_GROUPS 3
enum GroupType : uint32
{
	MEDIC = 0,
	SMOKE_DIVER = 1,
	NONE = 2
};

class TileData;

class Crewmember : public GameObject, public IOrderListener
{
	friend class Crew;

public:
	Crewmember(World* world, const glm::vec3& position, const std::string& name, GroupType groupType);
	~Crewmember();
	
	//UPDATES
	void Update(const Camera& camera, float deltaTime) noexcept override;
	void UpdateLastKnownPosition() noexcept;

	//PATHFINDING (NOT SETS OR GETS)
	void Move(const glm::vec3& dir, bool allowMult, float dtS);
	void FindPath(const glm::ivec3& goalPos);
	void LookForDoor() noexcept;
	void CloseDoorOrder(glm::ivec3 doorTile);
	void GoToMedicBay(World* world);

	//GAMEPLAY (NOT SETS OR GETS)
	bool Heal(int8 skillLevel, float dtS);
	void ApplyBurnInjury(float burn);
	void ApplyBoneInjury(float boneBreak);
	void ApplyBleedInjury(float bleed);

	void OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept override;
	//UTILITY (NOT SETS OR GETS)
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept override;
	virtual void OnOrderStarted(bool idleOrder) noexcept override;
	virtual void OnAllOrdersFinished() noexcept override;
	virtual void OnAddedToScene(Scene* scene) noexcept override;
	void OnHovered() noexcept override;
	void OnNotHovered() noexcept override;
	void GiveOrder(IOrder* order) noexcept;

	//SETS
	void SetPosition(const glm::vec3& position) noexcept;
	void SetDirection(const glm::vec3& direction) noexcept;
	void SetAssisting(Crewmember* inNeed) noexcept;
	void SetIdleing(bool value) noexcept;
	void SetGroup(uint32 group) noexcept;
	void SetIsPicked(bool picked) noexcept;

	//GETS
	int32 GetShipNumber() const noexcept;
	const glm::vec3& GetLastKnownPosition() const noexcept;
	const glm::vec3& GetDirection() const noexcept;
	const glm::ivec3& GetTile() const noexcept;
	float GetMovementSpeed() const noexcept;

	uint32 GetGroup() const noexcept;
	int32 GetForgetfulness() const noexcept;

	//IS
	bool IsIdleing() const noexcept;
	bool IsHovered() const noexcept override;
	bool IsPicked() const noexcept override;
	bool IsAlive() const noexcept;
	bool IsAbleToWork() const noexcept;
	bool IsAbleToWalk() const noexcept;

	//HAS
	bool HasInjuryBoneBroken() const noexcept;
	bool HasInjuryBurned() const noexcept;
	bool HasInjurySmoke() const noexcept;
	bool HasInjuryBleed() const noexcept;

private:
	//UPDATES
	void UpdateHealth(float dt);

	//GAMEPLAY
	///<summary>Updates the damage on the crewmember based on what level of smoke is on this members tile..</summary>
	void CheckSmokeDamage(const TileData*const* data, float dt) noexcept;
	///<summary>Updates the damage on the crewmember based on what level of fire is on this members tile..</summary>
	void CheckFireDamage(const TileData*const* data, float dt) noexcept;

	//SETS
	void SetShipNumber(int32 shipnumber) noexcept;

private:
	//UTILITY
	World* m_pWorld;
	glm::ivec3 m_PlayerTile;
	glm::vec3 m_Direction;
	bool m_IsPicked;
	bool m_IsHovered;

	//ORDERS
	OrderHandler m_OrderHandler;

	//GAMEPLAY
	//--SOME SUBCATEGORY?
	int32 m_ShipNumber;
	glm::vec3 m_LastKnownPosition;

	//--GROUP
	uint32 m_Group;
		
	//--INJURIES
	float m_HasInjuryBoneBroken;
	float m_HasInjuryBurned;
	float m_HasInjurySmoke;
	float m_HasInjuryBleeding;
	float m_MaxHealth;
	float m_Health;

	//--MOVEMENT
	float m_MovementSpeed;
	bool m_Idleing;

	int32 m_Forgetfulness;

	Crewmember* m_pAssisting;
	UISelectedCrew* m_pUISelectedCrew;
};

inline int32 Crewmember::GetShipNumber() const noexcept
{
	return m_ShipNumber;
}

inline const glm::vec3& Crewmember::GetLastKnownPosition() const noexcept
{
	return m_LastKnownPosition;
}

inline const glm::vec3& Crewmember::GetDirection() const noexcept
{
	return m_Direction;
}

inline const glm::ivec3& Crewmember::GetTile() const noexcept
{
	return m_PlayerTile;
}

inline float Crewmember::GetMovementSpeed() const noexcept
{
	return m_MovementSpeed;
}

inline uint32 Crewmember::GetGroup() const noexcept
{
	return m_Group;
}

inline int32 Crewmember::GetForgetfulness() const noexcept
{
	return m_Forgetfulness;
}

inline bool Crewmember::IsIdleing() const noexcept
{
	return m_Idleing;
}

inline bool Crewmember::IsHovered() const noexcept
{
	return m_IsHovered;
}

inline bool Crewmember::IsPicked() const noexcept
{
	return m_IsPicked;
}

inline bool Crewmember::IsAlive() const noexcept
{
	return m_Health > 0.0f;
}

inline bool Crewmember::IsAbleToWork() const noexcept
{
	return !(m_HasInjurySmoke > 5.0f || (m_Health < 0.75*m_MaxHealth) || m_HasInjuryBoneBroken >= 3.0f || m_HasInjuryBurned > 3.0f);
}

inline bool Crewmember::IsAbleToWalk() const noexcept
{
	return !(m_HasInjuryBoneBroken > 5.0f || m_HasInjurySmoke > 10.0f || m_Health < 0.5f * m_MaxHealth || m_HasInjuryBurned > 10.0f);
}

inline bool Crewmember::HasInjuryBoneBroken() const noexcept
{
	return m_HasInjuryBoneBroken > 1.0f;
}

inline bool Crewmember::HasInjuryBurned() const noexcept
{
	return m_HasInjuryBurned > 9.0f;
}

inline bool Crewmember::HasInjurySmoke() const noexcept
{
	return m_HasInjurySmoke > 9.0f;
}

inline bool Crewmember::HasInjuryBleed() const noexcept
{
	return m_HasInjuryBleeding > 1.0f;
}
