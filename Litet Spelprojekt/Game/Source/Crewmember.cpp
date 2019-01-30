#include "..\Include\Crewmember.h"

Crewmember::Crewmember(const glm::vec4 & lightColor, const glm::vec3 & position, const float & actionCap, const std::string & name) : m_pLight(new PointLight(position, lightColor)), m_pMesh(IndexedMesh::CreateCube())
{
	m_ActionCap = actionCap;
	m_Name = name;
	m_pMaterial = new Material();
	m_pMaterial->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	this->SetMaterial(m_pMaterial);
	this->SetMesh(m_pMesh);
	this->SetPosition(position);
	this->UpdateTransform();
}

Crewmember::Crewmember(Crewmember & other): m_pLight(new PointLight(other.GetPosition(), other.m_pLight->GetColor())), m_pMesh(other.m_pMesh)
{
	m_ActionCap = other.m_ActionCap;
	m_Name = other.m_Name;
	m_pMaterial = new Material(*other.m_pMaterial);
	this->SetMaterial(m_pMaterial);
	this->SetMesh(m_pMesh);
	this->SetPosition(other.GetPosition());
	this->UpdateTransform();
}

Crewmember::~Crewmember()
{
	delete m_pMaterial;
	delete m_pMesh;
}

void Crewmember::Move(const glm::vec3 & dir)
{
	glm::vec3 res = this->GetPosition() + dir;
	GameObject::SetPosition(res);
}

PointLight * Crewmember::GetLight() const
{
	return m_pLight;
}

const float Crewmember::GetActionCapacity() const
{
	return m_ActionCap;
}

void Crewmember::SetActionCapacity(const float actionCap)
{
	m_ActionCap = actionCap;
}

void Crewmember::UpdateTransform() noexcept
{
	GameObject::UpdateTransform();
	m_pLight->SetPosition(GetPosition());
}
