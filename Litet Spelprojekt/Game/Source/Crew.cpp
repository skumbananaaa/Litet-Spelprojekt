#include "..\Include\Crew.h"
#include "../Include/Orders/OrderHandler.h"

void Crew::SortGroups()
{
	m_Medics.clear();
	m_SmokeDivers.clear();
	m_None.clear();

	for (uint32 i = 0; i < m_NrOf; i++)
	{
		switch (m_ppMembers[i]->GetGroup())
		{
		case NONE:
			m_None.push_back(i);
			break;
		case SMOKE_DIVER:
			m_SmokeDivers.push_back(i);
			break;
		case MEDIC:
			m_Medics.push_back(i);
			break;
		}
	}
}

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

void Crew::AddSquad() noexcept
{
	m_Squads.push_back(std::vector<uint32>());
}

void Crew::RemoveFromSquad(uint32 squadIndex, uint32 memberIndex)
{
	bool res = false;
	assert(squadIndex < m_Squads.size());
	assert(squadIndex < m_Squads[squadIndex].size());
	m_Squads[squadIndex].erase(m_Squads[squadIndex].begin() + memberIndex);
}

void Crew::RemoveFromSelectedList(int32 crewIndex) noexcept
{
	m_SelectedMembers.erase(std::find(m_SelectedMembers.begin(), m_SelectedMembers.end(), crewIndex));
}

bool Crew::HasSelectedMembers() const noexcept
{
	return m_SelectedMembers.size() > 0;
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

const std::vector<uint32>& Crew::GetMembersOfGroup(uint32 group) const noexcept
{
	switch (group)
	{
	case NONE:
		return m_None;
		break;
	case SMOKE_DIVER:
		return m_SmokeDivers;
		break;
	case MEDIC:
		return m_Medics;
		break;
	}

	return m_None;
}

Crewmember * Crew::GetMemberFromSquad(uint32 squadIndex, uint32 memberIndex) const noexcept
{
	assert(squadIndex < m_Squads.size());
	assert(memberIndex < m_Squads[squadIndex].size());
	return m_ppMembers[m_Squads[squadIndex][memberIndex]];
}

Crewmember * Crew::GetMemberFromGroup(uint32 memberGroup, uint32 memberIndex) const noexcept
{
	assert(memberGroup < NR_GROUPS);
	uint32 index = -1;
	switch (memberGroup)
	{
	case NONE:
		assert(index < m_None.size());
		index = m_None[memberIndex];
		break;
	case SMOKE_DIVER:
		assert(index < m_SmokeDivers.size());
		index = m_SmokeDivers[memberIndex];
		break;
	case MEDIC:
		assert(index < m_Medics.size());
		index = m_Medics[memberIndex];
		break;
	}
	assert(index == -1);
	return m_ppMembers[index];
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

	for (uint32 i = 0; i < 8; i++)
	{
		m_Squads.push_back(std::vector<uint32>());
	}
}

Crew::~Crew()
{
	DeleteArrSafe(m_ppMembers);
}

void Crew::AddMember(World* world, const glm::vec4& lightColor, const glm::vec3& position, float actionCap, const std::string& name, uint32 group, uint32 squad)
{
	assert(squad < m_Squads.size());
	assert(group < NR_GROUPS);
	Expand();
	Crewmember* member = new Crewmember(world, lightColor, position, actionCap, name);
	member->SetShipNumber(m_NrOf);

	switch (group)
	{
	case NONE:
		m_None.push_back(m_NrOf);
		member->m_Group = NONE;
		break;
	case SMOKE_DIVER:
		m_SmokeDivers.push_back(m_NrOf);
		member->m_Group = SMOKE_DIVER;
		break;
	case MEDIC:
		m_Medics.push_back(m_NrOf);
		member->m_Group = MEDIC;
		break;
	}
	m_Squads[squad].push_back(m_NrOf);

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

const uint32 Crew::GetSquadCount(uint32 squad) const
{
	assert(m_Squads.size() < squad);
	return m_Squads[squad].size();
}

const uint32 Crew::GetGroupCount(uint32 group) const
{
	assert(group < NR_GROUPS);
	switch (group)
	{
	case NONE:
		return m_None.size();
		break;
	case SMOKE_DIVER:
		return m_SmokeDivers.size();
		break;
	case MEDIC:
		return m_Medics.size();
		break;
	}
}

const uint32 Crew::NrOfSquads() const
{
	return m_Squads.size();
}
