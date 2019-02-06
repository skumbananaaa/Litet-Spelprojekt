#ifndef CREWMEMBER_H
#define CREWMEMBER_H
#include <Graphics/Lights/PointLight.h>
#include <Graphics/GameObject.h>
#include <System/ThreadHandler.h>
#include <string>
#include "..\Include\Path.h"

class Crewmember : public GameObject, public IRunnable
{
public:
	Crewmember(const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "");
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
	void SetPath(const World* world);
	///<summary>Returns a reference to the pointLight object.</summary>
	PointLight * GetLight() const;
	///<summary>returns the current action capacity of the crewmember.</summary>
	const float GetActionCapacity() const;
	///<summary>Returns true if the object is currently moving. Otherwise returns false.</summary>
	const bool IsMoving() const;
	///<summary>Sets a new position for the object.</summary>
	void SetPosition(const glm::vec3& position) noexcept;
	const glm::vec3& GetDirection() const noexcept;
	void SetDirection(const glm::vec3& direction) noexcept;

	virtual void RunParallel() override;
	///<summary>Updates matrix of the object.</summary>
	virtual void Update(float deltaTime) override;

private:
	float m_ActionCap;
	float m_DeltaTime;
	std::string m_Name;
	PointLight * m_pLight;
	Path* m_pPathFinder;
	glm::ivec3 m_PlayerTile;
	glm::ivec3 m_TargetTile;
	glm::ivec3 m_GoalTile;
	glm::ivec3* m_pPath;
	glm::vec3 m_TargetPos;
	glm::vec3 m_Direction;
	int m_NrOfPathTiles;
};

#endif
