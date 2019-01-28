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
	Crewmember(const glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), const float actionCap = 100, const std::string name = "");
	~Crewmember();
	PointLight * getLight() const;
	void move(const glm::vec3 & dir);
	void setPosition(const glm::vec3 & dir);
	const glm::vec3 getPos() const;
	void update();
};

#endif
