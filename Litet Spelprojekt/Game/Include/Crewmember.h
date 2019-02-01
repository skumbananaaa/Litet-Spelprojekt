#ifndef CREWMEMBER_H
#define CREWMEMBER_H
#include <Graphics/Lights/PointLight.h>
#include <Graphics/GameObject.h>
#include <string>
#include "..\Include\Path.h"

class Crewmember : public GameObject
{
private:
	float m_ActionCap;
	std::string m_Name;
	Material * m_pMaterial;
	PointLight * m_pLight;
	IndexedMesh * m_pMesh;
	Path* m_pPathFinder;
	glm::ivec2 m_playerTile;
	glm::ivec2 m_targetTile;
	glm::ivec2* m_pPath;
	glm::vec3 m_targetPos;
	int m_nrOfPathTiles;

public:

	Crewmember(const glm::vec4 & lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const float & actionCap = 100, const std::string & name = "");
	Crewmember(Crewmember & other);
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
	void SetPath(const World* world, uint32 level);
	///<summary>Returns a reference to the pointLight object.</summary>
	PointLight * GetLight() const;
	///<summary>returns the current action capacity of the crewmember.</summary>
	const float GetActionCapacity() const;
	///<summary>Returns true if the object is currently moving. Otherwise returns false.</summary>
	const bool IsMoving() const;
	///<summary>Sets a new position for the object.</summary>
	void SetPosition(const glm::vec3& position) noexcept;
	///<summary>Updates matrix of the object.</summary>
	void UpdateTransform() noexcept;
};

#endif
