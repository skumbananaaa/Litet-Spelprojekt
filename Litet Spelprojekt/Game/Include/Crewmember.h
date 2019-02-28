#ifndef CREWMEMBER_H
#define CREWMEMBER_H
#include <EnginePch.h>
#include <Graphics/Lights/PointLight.h>
#include <Graphics/Lights/SpotLight.h>
#include <Graphics/GameObject.h>
#include <System/ThreadHandler.h>
#include "..\Include\Path.h"

#define CHOSEN_LIGHT glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_LIGHT glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)

class Crewmember : public GameObject, public IRunnable
{
	friend class Crew;

public:
	Crewmember(const World* world, const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "");
	Crewmember(Crewmember& other);
	~Crewmember();
	
	///<summary>Moves the objects position in the given direction, use update to apply.</summary>
	void Move(const glm::vec3 & dir);
	///<summary>Finds a path to the goal position.</summary>
	void FindPath(const glm::ivec3& goalPos);
	///<summary>Moves along the path towards the goal position.</summary>
	void FollowPath(float dtS);
	///<summary>Sets the actioncapacity of the crewmember to the specified value.</summary>
	void SetActionCapacity(float actionCap);
	///<summary>Sets a grid for the path finding algorithm.</summary>
	void SetPath();
	///<summary>Returns a reference to the PointLight object.</summary>
	PointLight* GetLight() const;
	///<summary>Returns a reference to the SpotLight object.</summary>
	SpotLight* GetTorch() const;
	///<summary>returns the current action capacity of the crewmember.</summary>
	const float GetActionCapacity() const;
	///<summary>Returns true if the object is currently moving. Otherwise returns false.</summary>
	const bool IsMoving() const;
	///<summary>Sets a new position for the object.</summary>
	void SetPosition(const glm::vec3& position) noexcept;
	///<summary>Gets the current direction that the crewmember is facing.</summary>
	const glm::vec3& GetDirection() const noexcept;
	///<summary>Sets the current direction that the crewmember is facing. Use .Update() to apply visual changes.</summary>
	void SetDirection(const glm::vec3& direction) noexcept;
	///<summary>>Switches between point light and spotlight for the crewmember.</summary>
	void SwitchLight() noexcept;
	glm::ivec3 GetTile() const noexcept;
	///<summary>Used to determine path of crewmember to an already selected target. Function made to run on multiple threads.</summary>
	virtual void RunParallel() override;
	///<summary>Updates matrix of the object.</summary>
	virtual void Update(const Camera& camera, float deltaTime) noexcept override;

	virtual void OnPicked();
	virtual void OnHovered();
	virtual void OnNotHovered();
	void UpdateLastKnownPosition() noexcept;
	const glm::vec3& GetLastKnownPosition() const noexcept;
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept;

	int32 GetShipNumber() const noexcept;

	bool IsHovered() const noexcept;

	int8 GetSkillFire() const noexcept;
	int8 GetSkillMedic() const noexcept;
	int8 GetSkillStrength() const noexcept;
	bool HasInjuryBoneBroken() const noexcept;
	bool HasInjuryBurned() const noexcept;
	bool HasInjurySmoke() const noexcept;
	bool isAlive() const noexcept;
private:
	void SetShipNumber(int32 shipnumber) noexcept;
	///<summary>Updates the damage on the crewmember based on what level of smoke is on this members tile..</summary>
	void CheckSmokeDamage(const TileData*const* data) noexcept;
	///<summary>Updates the damage on the crewmember based on what level of fire is on this members tile..</summary>
	void CheckFireDamage(const TileData*const* data) noexcept;
	void UpdateHealth(float dt);
	float m_ActionCap;
	float m_DeltaTime;
	SpotLight* m_pTorch;
	PointLight* m_pLight;
	Path* m_pPathFinder;
	const World* m_pWorld;
	glm::ivec3 m_PlayerTile;
	glm::ivec3 m_TargetTile;
	glm::ivec3 m_GoalTile;
	glm::ivec3* m_pPath;
	glm::vec3 m_TargetPos;
	glm::vec3 m_Direction;
	glm::vec3 m_LastKnownPosition;
	int m_NrOfPathTiles;
	bool m_IsHovered;
	int32 m_ShipNumber;

	int8 m_SkillFire;
	int8 m_SkillMedic;
	int8 m_SkillStrength;
	bool m_HasInjuryBoneBroken;
	float m_HasInjuryBurned;
	float m_HasInjurySmoke;
	float m_Health;
};

#endif
