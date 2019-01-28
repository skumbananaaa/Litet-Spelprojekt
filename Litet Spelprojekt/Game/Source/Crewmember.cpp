#include "..\Include\Crewmember.h"

Crewmember::Crewmember(const glm::vec4 lightColor, const glm::vec3 position, const float actionCap, const std::string name) : m_pLight(new PointLight(lightColor, position)), m_pMesh(IndexedMesh::CreateCube())
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

Crewmember::~Crewmember()
{
	delete m_pMaterial;
	delete m_pMesh;
}

PointLight * Crewmember::getLight() const
{
	return m_pLight;
}

void Crewmember::move(const glm::vec3 & dir)
{
	glm::vec3 res = this->GetPosition() + dir;
	SetPosition(res);
}

void Crewmember::setPosition(const glm::vec3 & position)
{
	SetPosition(position);
	UpdateTransform();
}

const glm::vec3 Crewmember::getPos() const
{
	return GetPosition();
}

void Crewmember::update()
{
	UpdateTransform();
	m_pLight->SetPosition(GetPosition());
}
