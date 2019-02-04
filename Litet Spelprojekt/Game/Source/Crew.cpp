#include "..\Include\Crew.h"

void Crew::expand()
{
	if (m_nrOf == m_cap)
	{
		m_cap *= 2;

		Crewmember ** tmp = new Crewmember*[m_cap];
		for (int i = 0; i < m_nrOf; i++)
		{
			tmp[i] = m_ppMembers[i];
		}

		for (int i = m_nrOf; i < m_cap; i++)
		{
			tmp[i] = nullptr;
		}
		delete[] m_ppMembers;
		m_ppMembers = tmp;
	}
}

Crew::Crew(int cap)
{
	m_cap = cap;
	m_nrOf = 0;
	m_ppMembers = new Crewmember*[cap];
	for (int i = 0; i < cap; i++)
	{
		m_ppMembers[i] = nullptr;
	}
}

Crew::~Crew()
{
	DeleteArr(m_ppMembers);
}

void Crew::addMember(const glm::vec4 & lightColor, const glm::vec3 & position, const float & actionCap, const std::string & name)
{
	expand();
	m_ppMembers[m_nrOf++] = new Crewmember(lightColor, position, actionCap, name);
}

Crewmember * Crew::getMember(int index)
{
	return m_ppMembers[index];
}

const int Crew::getCount() const
{
	return m_nrOf;
}
