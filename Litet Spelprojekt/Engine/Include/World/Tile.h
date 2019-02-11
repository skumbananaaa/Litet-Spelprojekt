#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>

class API Tile : public GameObject
{
public:
	Tile(uint32 material, const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), uint32 id = 0);
	~Tile();

	///<summary>Sets the id of the tile.</summary>
	void SetID(uint32 id);
	///<summary>Sets the current and default material, this material gets set as current when ResetMaterial is called. </summary>
	void SetDefaultMaterial(uint32 material) noexcept;
	///<summary>Sets the current material to the default material. </summary>
	void ResetMaterial() noexcept;
	void SetHasStairs(bool value) noexcept;

	///<summary>Returns the id of the tile.</summary>
	uint32 GetID() const;
	///<summary>Returns the id of the tile.</summary>
	bool HasStairs() const;

private:
	uint32 m_Id;
	uint32 m_DefaultMaterial;
	bool m_HasStairs;
};

inline uint32 Tile::GetID() const
{
	return m_Id;
}

inline bool Tile::HasStairs() const
{
	return m_HasStairs;
}
