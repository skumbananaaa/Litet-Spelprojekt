#include "..\Include\Crew.h"

Crew::Crew()
{
	for(int i = 0; i < 2; i++)
	{
		m_Squads.push_back(std::vector<uint32>());
	}
}

Crew::~Crew()
{
	
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

void Crew::ClearSelectedList() noexcept
{
	for (int i = 0; i < m_SelectedMembers.size(); i++)
	{
		GetMember(m_SelectedMembers[i])->SetIsPicked(false);
	}
	m_SelectedMembers.clear();
}

void Crew::RemoveFromSelectedList(int32 crewIndex) noexcept
{
	for (int i = 0; i < m_SelectedMembers.size(); i++)
	{
		if (m_SelectedMembers[i] == crewIndex)
		{
			GetMember(m_SelectedMembers[i])->SetIsPicked(false);
			m_SelectedMembers.erase(m_SelectedMembers.begin() + i);
			return;
		}
	}
}

bool Crew::HasSelectedMembers() const noexcept
{
	return !m_SelectedMembers.empty();
}

const std::vector<int32>& Crew::GetSelectedList() const noexcept
{
	return m_SelectedMembers;
}

const std::vector<uint32>& Crew::GetSquad(uint32 index) const noexcept
{
	assert(index < m_Squads.size());
	return m_Squads[index];
}

void Crew::AddMember(World* world, const glm::vec3& position, const std::string& name, GroupType groupType)
{
	Crewmember* member = new Crewmember(world, position, name, groupType);
	member->SetShipNumber(m_Members.size());

	if (groupType != NONE)
	{
		m_Squads[groupType].push_back(m_Members.size());
	}

	int32 squad = m_Members.size() / 2 + 2;
	if (m_Squads.size() == squad)
	{
		m_Squads.push_back(std::vector<uint32>());
	}
	m_Squads[squad].push_back(m_Members.size());

	m_Members.push_back(member);
}

Crewmember* Crew::GetMember(int index)
{
	return m_Members[index];
}

const int Crew::GetCount() const
{
	return m_Members.size();
}

const uint32 Crew::GetSquadCount(uint32 squad) const
{
	assert(squad < m_Squads.size());
	return m_Squads[squad].size();
}

const uint32 Crew::NrOfSquads() const
{
	return m_Squads.size();
}
