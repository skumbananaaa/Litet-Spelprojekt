#ifndef CREW_H
#define CREW_H
#include "Crewmember.h"
#define NR_OF_SQUADS = 8


class Crew
{
public:
	Crew(int cap = 10);
	~Crew();

	void RequestCloseDoor(World* pWorld, Scene* pScene);
	bool HasSelectedMembers() const noexcept;
	
	// Adds
	void AddMember(World* world, const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "", uint32 group = NONE, uint32 squad = 0);
	void AddToSelectedList(int32 crewIndex) noexcept;
	void AddSquad() noexcept;

	// Removes
	void RemoveFromSquad(uint32 squadIndex, uint32 memberIndex);

	// Gets
		// Vectors
	const std::vector<int32>& GetSelectedList() const noexcept;
	const std::vector<uint32>& GetSquad(uint32 index) const noexcept;
	const std::vector<uint32>& GetMembersOfGroup(uint32 group) const noexcept;
		// Crewmembers
	Crewmember* GetMemberFromSquad(uint32 squadIndex, uint32 memberIndex) const noexcept;
	Crewmember* GetMemberFromGroup(uint32 memberGroup, uint32 memberIndex) const noexcept;
	Crewmember* GetMember(int index);
		// Counts
	const int GetCount() const;
	const uint32 GetSquadCount(uint32 squad) const;
	const uint32 GetGroupCount(uint32 group) const;
	const uint32 NrOfSquads() const;


	// Self checks
	void SortGroups();
private:
	void Expand();

private:
	int m_NrOf;
	int m_Cap;
	Crewmember** m_ppMembers;
	std::vector<int32> m_SelectedMembers;

	std::vector<uint32> m_SmokeDivers;
	std::vector<uint32> m_Medics;
	std::vector<uint32> m_None;

	std::vector<std::vector<uint32>> m_Squads;
};

#endif