#pragma once

#include "Crewmember.h"

class Crew
{
	friend class Crewmember;

public:
	Crew();
	~Crew();

	bool HasSelectedMembers() const noexcept;
	
	// Adds
	void AddMember(World* world, const glm::vec3& position, const std::string& name, GroupType groupType);

	// Gets
	const std::vector<int32>& GetSelectedList() const noexcept;
	const std::vector<uint32>& GetSquad(uint32 index) const noexcept;

	void ClearSelectedList() noexcept;


	// Crewmembers
	Crewmember* GetMember(int index);

	// Counts
	const int GetCount() const;
	const uint32 GetSquadCount(uint32 squad) const;
	const uint32 NrOfSquads() const;

private:
	void AddToSelectedList(int32 crewIndex) noexcept;
	void RemoveFromSelectedList(int32 crewIndex) noexcept;

	std::vector<Crewmember*> m_Members;

	std::vector<int32> m_SelectedMembers;
	std::vector<std::vector<uint32>> m_Squads;
};