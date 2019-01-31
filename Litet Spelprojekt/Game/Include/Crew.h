#ifndef CREW_H
#define CREW_H
#include "Crewmember.h"
class Crew
{
private:
	Crewmember ** m_ppMembers;
	int m_cap;
	int m_nrOf;
	void expand();
public:
	Crew(int cap = 10);
	~Crew();
	void addMember(const glm::vec4 & lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3 & position = glm::vec3(0.0f, 0.0f, 0.0f), const float & actionCap = 100, const std::string & name = "");
	Crewmember * getMember(int index);
	const int getCount() const;
};

#endif