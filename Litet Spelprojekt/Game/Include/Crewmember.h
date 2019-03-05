#ifndef CREWMEMBER_H
#define CREWMEMBER_H

#include <Graphics/GameObject.h>
#include "../Include/Orders/OrderHandler.h"

#define CHOSEN_LIGHT glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_LIGHT glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)
#define MAX_NR_OF_ORDERS 10

#define CREWMEMBER_FULL_HEALTH_MOVEMENT_SPEED 2.5f
#define CREWMEMBER_LIGHTLY_INJURED_MOVEMENT_SPEED 1.8f
#define CREWMEMBER_SERIOUSLY_INJURED_MOVEMENT_SPEED 0.8f
#define CREWMEMBER_DEAD_MOVEMENT_SPEED 0.0f

#define CREWMEMBER_IDLE_MOVEMENT_SPEED_MULTIPLIER 0.7f

class TileData;

class Crewmember : public GameObject, public IOrderListener
{
	friend class Crew;

public:
	Crewmember(World* world, const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "");
	~Crewmember();
	
	//UPDATES
	void Update(const Camera& camera, float deltaTime) noexcept override;
	void UpdateLastKnownPosition() noexcept;

	//PATHFINDING (NOT SETS OR GETS)
	void Move(const glm::vec3& dir, bool allowMult, float dtS);
	void FindPath(const glm::ivec3& goalPos);
	void LookForDoor() noexcept;
	void CloseDoorOrder(glm::ivec3 doorTile);

	//GAMEPLAY (NOT SETS OR GETS)
	bool Heal(int8 skillLevel);
	void ApplyBurnInjury(float burn);
	void ApplyBoneInjury();

	//UTILITY (NOT SETS OR GETS)
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept override;
	virtual void OnOrderStarted() noexcept override;
	virtual void OnAllOrdersFinished() noexcept override;
	virtual void OnAddedToScene(Scene* scene) noexcept override;
	void OnPicked() noexcept override;
	void OnHovered() noexcept override;
	void OnNotHovered() noexcept override;

	//SETS
	void SetPosition(const glm::vec3& position) noexcept;
	void SetDirection(const glm::vec3& direction) noexcept;
	void SetAssisting(Crewmember* inNeed) noexcept;
	void SetIdleing(bool value) noexcept;

	//GETS
	int32 GetShipNumber() const noexcept;
	const glm::vec3& GetLastKnownPosition() const noexcept;
	const glm::vec3& GetDirection() const noexcept;
	const glm::ivec3& GetTile() const noexcept;
	float GetMovementSpeed() const noexcept;
	int8 GetSkillFire() const noexcept;
	int8 GetSkillMedic() const noexcept;
	int8 GetSkillStrength() const noexcept;

	//IS
	bool IsIdleing() const noexcept;
	bool IsHovered() const noexcept override;
	bool IsPicked() const noexcept override;
	bool isAlive() const noexcept;

	//HAS
	bool HasInjuryBoneBroken() const noexcept;
	bool HasInjuryBurned() const noexcept;
	bool HasInjurySmoke() const noexcept;

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

	//--SKILLS
	int8 m_SkillFire;
	int8 m_SkillMedic;
	int8 m_SkillStrength;
		
	//--INJURIES
	bool m_HasInjuryBoneBroken;
	float m_HasInjuryBurned;
	float m_HasInjurySmoke;
	float m_MaxHealth;
	float m_Health;

	//--MOVEMENT
	float m_MovementSpeed;
	bool m_Idleing;

	Crewmember* m_pAssisting;
};

#endif

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

const glm::ivec3& Crewmember::GetTile() const noexcept
{
	return m_PlayerTile;
}

inline float Crewmember::GetMovementSpeed() const noexcept
{
	return m_MovementSpeed;
}

inline int8 Crewmember::GetSkillFire() const noexcept
{
	return m_SkillFire;
}

inline int8 Crewmember::GetSkillMedic() const noexcept
{
	return m_SkillMedic;
}

inline int8 Crewmember::GetSkillStrength() const noexcept
{
	return m_SkillStrength;
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

inline bool Crewmember::isAlive() const noexcept
{
	return m_Health > 0.0f;
}

inline bool Crewmember::HasInjuryBoneBroken() const noexcept
{
	return m_HasInjuryBoneBroken;
}

inline bool Crewmember::HasInjuryBurned() const noexcept
{
	return m_HasInjuryBurned > 1.0f;
}

inline bool Crewmember::HasInjurySmoke() const noexcept
{
	return m_HasInjurySmoke > 1.0f;
}