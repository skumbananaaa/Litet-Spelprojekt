#ifndef CREW_H
#define CREW_H
#include "Crewmember.h"

class Crew
{
public:
	Crew(int cap = 10);
	~Crew();

	void AddMember(const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "");
	Crewmember* GetMember(int index);
	const int GetCount() const;

private:
	void Expand();

private:
	Crewmember** m_ppMembers;
	int m_NrOf;
	int m_Cap;
};

#endif