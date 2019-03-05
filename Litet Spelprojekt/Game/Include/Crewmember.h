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

class Crewmember : public GameObject
{
	friend class Crew;

public:
	Crewmember(World* world, const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "");
	Crewmember(Crewmember& other);
	~Crewmember();
	
	///<summary>Moves the objects position in the given direction, use update to apply.</summary>
	void Move(const glm::vec3& dir, bool allowMult, float dtS);
	///<summary>Finds a path to the goal position.</summary>
	void FindPath(const glm::ivec3& goalPos);

	void LookForDoor() noexcept;
	virtual void OnAddedToScene(Scene* scene) noexcept override;
	void CloseDoorOrder(glm::ivec3 doorTile);

	///<summary>Sets the actioncapacity of the crewmember to the specified value.</summary>
	void SetActionCapacity(float actionCap);
	///<summary>Sets a new position for the object.</summary>
	void SetPosition(const glm::vec3& position) noexcept;
	///<summary>Sets the current direction that the crewmember is facing. Use .Update() to apply visual changes.</summary>
	void SetDirection(const glm::vec3& direction) noexcept;
	void SetAssisting(Crewmember* inNeed) noexcept;
	void SetIdleing(bool value) noexcept;

	void OnPicked() noexcept override;
	void OnHovered() noexcept override;
	void OnNotHovered() noexcept override;
	bool IsHovered() const noexcept override;
	bool IsPicked() const noexcept override;

	void UpdateLastKnownPosition() noexcept;
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept override;


	int32 GetShipNumber() const noexcept;
	///<summary>returns the current action capacity of the crewmember.</summary>
	const float GetActionCapacity() const;
	const glm::vec3& GetLastKnownPosition() const noexcept;
	///<summary>Gets the current direction that the crewmember is facing.</summary>
	const glm::vec3& GetDirection() const noexcept;
	float GetMovementSpeed() const noexcept;
	bool IsIdleing() const noexcept;
	glm::ivec3 GetTile() const noexcept;



	int8 GetSkillFire() const noexcept;
	int8 GetSkillMedic() const noexcept;
	int8 GetSkillStrength() const noexcept;
	bool HasInjuryBoneBroken() const noexcept;
	bool HasInjuryBurned() const noexcept;
	bool HasInjurySmoke() const noexcept;
	bool isAlive() const noexcept;
	void ApplyBurnInjury(float burn);
	void ApplyBoneInjury();

	bool Heal(int8 skillLevel);
	
private:
	void SetShipNumber(int32 shipnumber) noexcept;
	///<summary>Updates the damage on the crewmember based on what level of smoke is on this members tile..</summary>
	void CheckSmokeDamage(const TileData*const* data, float dt) noexcept;
	///<summary>Updates the damage on the crewmember based on what level of fire is on this members tile..</summary>
	void CheckFireDamage(const TileData*const* data, float dt) noexcept;
	void UpdateHealth(float dt);
	float m_ActionCap;
	float m_DeltaTime;
	World* m_pWorld;
	glm::ivec3 m_PlayerTile;
	glm::vec3 m_Direction;
	glm::vec3 m_LastKnownPosition;
	bool m_IsPicked;
	bool m_IsHovered;
	int32 m_ShipNumber;
	OrderHandler m_OrderHandler;

	int8 m_SkillFire;
	int8 m_SkillMedic;
	int8 m_SkillStrength;
	bool m_HasInjuryBoneBroken;
	float m_HasInjuryBurned;
	float m_HasInjurySmoke;
	float m_MaxHealth;
	float m_Health;
	float m_MovementSpeed;
	bool m_Idleing;

	Crewmember* m_pAssisting;
};

#endif