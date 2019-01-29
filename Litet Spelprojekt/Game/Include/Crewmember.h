#ifndef CREWMEMBER_H
#define CREWMEMBER_H
#include <Graphics/Lights/PointLight.h>
#include <Graphics/GameObject.h>
#include <string>
class Crewmember : public GameObject
{
private:
	float m_ActionCap;
	std::string m_Name;
	Material * m_pMaterial;
	PointLight * m_pLight;
	IndexedMesh * m_pMesh;
public:

	Crewmember(const glm::vec4 & lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const float & actionCap = 100, const std::string & name = "");
	Crewmember(Crewmember & other);
	~Crewmember();
	///<summary>Moves the objects position in the given direction, use update to apply.</summary>
	void move(const glm::vec3 & dir);
	///<summary>Moves the object to the specified position.</summary>
	void setPosition(const glm::vec3 & position);
	///<summary>Sets the actioncapacity of the crewmember to the specified value.</summary>
	void setActionCapacity(const float actionCap);
	///<summary>Returns the position of the object.</summary>
	const glm::vec3 getPos() const;
	///<summary>Returns a reference to the pointLight object.</summary>
	PointLight * getLight() const;
	///<summary>returns the current action capacity of the crewmember.</summary>
	const float getActionCapacity() const;
	///<summary>Updates matrix of the object.</summary>
	void update();
};

#endif
