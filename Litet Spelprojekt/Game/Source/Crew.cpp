#include "..\Include\Crew.h"

void Crew::Expand()
{
	if (m_NrOf == m_Cap)
	{
		m_Cap *= 2;

		Crewmember** tmp = new Crewmember*[m_Cap];
		for (int i = 0; i < m_NrOf; i++)
		{
			tmp[i] = m_ppMembers[i];
		}
		for (int i = m_NrOf; i < m_Cap; i++)
		{
			tmp[i] = nullptr;
		}

		DeleteArrSafe(m_ppMembers);

		m_ppMembers = tmp;
	}
}

Crew::Crew(int cap)
{
	m_Cap = cap;
	m_NrOf = 0;

	m_ppMembers = new Crewmember*[cap];
	for (int i = 0; i < cap; i++)
	{
		m_ppMembers[i] = nullptr;
	}
}

Crew::~Crew()
{
	DeleteArrSafe(m_ppMembers);
}

void Crew::AddMember(const glm::vec4& lightColor, const glm::vec3& position, float actionCap, const std::string& name)
{
	Expand();
	m_ppMembers[m_NrOf++] = new Crewmember(lightColor, position, actionCap, name);
}

Crewmember* Crew::GetMember(int index)
{
	return m_ppMembers[index];
}

const int Crew::GetCount() const
{
	return m_NrOf;
}
