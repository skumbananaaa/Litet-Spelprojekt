#include "..\Include\Crew.h"
#include "../Include/Orders/OrderHandler.h"

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

void Crew::RequestCloseDoor(World* pWorld, Scene* pScene)
{
	for (int i = 0; i < m_NrOf; i++)
	{
		m_ppMembers[i]->LookForDoor();
	}
}

void Crew::AddToSelectedList(int32 crewIndex) noexcept
{
	for (int i = 0; i < m_SelectedMembers.size(); i++)
	{
		if (m_SelectedMembers[i] == crewIndex)
		{
			return;
		}
	}
	m_SelectedMembers.push_back(crewIndex);
}

bool Crew::HasSelectedMembers() const noexcept
{
	return m_SelectedMembers.size() > 0;
}

const std::vector<int32>& Crew::GetSelectedList() const noexcept
{
	return m_SelectedMembers;
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

void Crew::AddMember(World* world, const glm::vec4& lightColor, const glm::vec3& position, float actionCap, const std::string& name)
{
	Expand();
	Crewmember* member = new Crewmember(world, lightColor, position, actionCap, name);
	member->SetShipNumber(m_NrOf);
	m_ppMembers[m_NrOf++] = member;
}

Crewmember* Crew::GetMember(int index)
{
	return m_ppMembers[index];
}

const int Crew::GetCount() const
{
	return m_NrOf;
}
