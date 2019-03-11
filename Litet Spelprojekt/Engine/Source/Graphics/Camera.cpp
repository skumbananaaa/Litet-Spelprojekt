#include <EnginePch.h>
#include <Graphics\Camera.h>

Camera::Camera(const glm::vec3& pos, float pitch, float yaw, const glm::vec3& upVector) noexcept :
	m_MinMaxPitch(-1.55334303f, 1.55334303f),
	m_MinMaxDistanceToLookAt(0.1f, 60.0f),
	m_MinXZMaxXZLookAt(glm::vec2(-FLT_MAX), glm::vec2(FLT_MAX))
{
	m_InverseIsDirty = true;
	m_Position = pos;
	m_Pitch = pitch;
	m_Yaw = yaw;
	m_Front = glm::normalize(glm::vec3(
		cosf(m_Pitch) * cosf(m_Yaw),
		sinf(m_Pitch),
		cosf(m_Pitch) * sinf(m_Yaw)));
	m_WorldUp = upVector;
	m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);
	m_LookAt = m_Position + m_Front;
	m_DistanceToLookAt = 1.0f;
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, m_WorldUp);

	CalcInverses();
	m_IsDirty = false;
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& upVector) noexcept :
	m_MinMaxPitch(-1.55334303f, 1.55334303f),
	m_MinMaxDistanceToLookAt(0.1f, 60.0f),
	m_MinXZMaxXZLookAt(glm::vec2(-FLT_MAX), glm::vec2(FLT_MAX))
{
	m_InverseIsDirty = true; 
	m_Position = pos;
	m_LookAt = lookAt;
	glm::vec3 posToLookAt = m_LookAt - m_Position;
	m_Front = glm::normalize(posToLookAt);
	m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);
	m_WorldUp = upVector;
	m_Pitch = asinf(m_Front.y);
	m_Yaw = atan2(m_Front.x, m_Front.z) - 3.0f * glm::half_pi<float>();
	m_DistanceToLookAt = glm::length(posToLookAt);
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, m_WorldUp);

	CalcInverses();
	m_IsDirty = false;
}

Camera::~Camera()
{
}

void Camera::UpdateFromPitchYaw() noexcept
{
	UpdateFromPitchYawInternal();
	CalcInverses();
}

void Camera::UpdateFromLookAt() noexcept
{
	UpdateFromLookAtInternal();
	CalcInverses();
}

void Camera::UpdateFromPitchYawNoInverse() noexcept
{
	UpdateFromPitchYawInternal();
}

void Camera::UpdateFromLookAtNoInverse() noexcept
{
	UpdateFromLookAtInternal();
}

/*void Camera::SetProjectionMatrix(const glm::mat4& matrix) noexcept
{
	m_ProjectionMatrix = matrix;
	m_IsDirty = true;
}*/

void Camera::CreateOrthographic(float windowWidth, float windowHeight, float nearPlane, float farPlane) noexcept
{
	float windowWidth_2 = windowWidth / 2.0f;
	float windowHeight_2 = windowHeight / 2.0f;
	m_ProjectionMatrix = glm::ortho(-windowWidth_2, windowWidth_2, -windowHeight_2, windowHeight_2, nearPlane, farPlane);
	m_Near = nearPlane;
	m_Far = farPlane;

	m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
	m_IsDirty = true;
	m_InverseIsDirty = true;
}

void Camera::CreatePerspective(float fovRad, float aspectWihe, float nearPlane, float farPlane) noexcept
{
	m_ProjectionMatrix = glm::perspective(fovRad, aspectWihe, nearPlane, farPlane);
	m_Near = nearPlane;
	m_Far = farPlane;

	m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
	m_IsDirty = true;
	m_InverseIsDirty = true;
}

void Camera::MoveWorldCoords(const glm::vec3& worldCoords, bool moveLookAt) noexcept
{
	glm::vec3 newLookAt = m_LookAt + worldCoords;

	if (IsLookAtInBounds(newLookAt))
	{
		m_IsDirty = true;
		m_InverseIsDirty = true;

		m_Position += worldCoords;

		if (moveLookAt)
		{
			m_LookAt = newLookAt;
		}

		m_DistanceToLookAt = glm::length(m_LookAt - m_Position);
	}
}

void Camera::MoveLocalCoords(const glm::vec3& localCoords, bool moveLookAt) noexcept
{
	glm::vec3 worldCoords(0.0f);
	glm::vec3 right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	glm::vec3 up = glm::normalize(glm::cross(m_Front, right));

	worldCoords -= right * localCoords.x;
	worldCoords -= up * localCoords.y;
	worldCoords += m_Front * localCoords.z;

	glm::vec3 newLookAt = m_LookAt + worldCoords;

	if (IsLookAtInBounds(newLookAt))
	{
		m_IsDirty = true;
		m_InverseIsDirty = true;

		m_Position += worldCoords;

		if (moveLookAt)
		{
			m_LookAt = newLookAt;
		}

		m_DistanceToLookAt = glm::length(m_LookAt - m_Position);
	}
}

void Camera::MoveRelativeLookAt(PosRelativeLookAt dir, float amount) noexcept
{
	m_IsDirty = true;
	m_InverseIsDirty = true;

	switch (dir)
	{
		case PosRelativeLookAt::Zoom:
		{
			glm::vec3 newPos = m_Position + m_Front * amount;
			glm::vec3 newToLookAt = m_LookAt - newPos;
			float distanceToLookAtAfterMove = glm::length(newToLookAt);
			glm::vec3 newFront = newToLookAt / distanceToLookAtAfterMove;

			if (amount > 0.0f)
			{
				if (distanceToLookAtAfterMove > m_MinMaxDistanceToLookAt.x && glm::dot(newFront, m_Front) > 0.9f)
				{
					m_Position = newPos;
					m_DistanceToLookAt = distanceToLookAtAfterMove;
				}
			}
			else if (distanceToLookAtAfterMove < m_MinMaxDistanceToLookAt.y)
			{
				m_Position = newPos;
				m_DistanceToLookAt = distanceToLookAtAfterMove;
			}

			break;
		}

		case PosRelativeLookAt::RotateX:
		{
			float distanceToLookAt = glm::length(glm::vec3(m_LookAt.x - m_Position.x, 0.0f, m_LookAt.z - m_Position.z));
			m_Yaw += amount;
			m_Position.x = m_LookAt.x - cosf(m_Yaw) * distanceToLookAt;
			m_Position.z = m_LookAt.z - sinf(m_Yaw) * distanceToLookAt;
			break;
		}

		case PosRelativeLookAt::RotateY:
		{
			float distanceToLookAt = glm::length(m_LookAt - m_Position);
			m_Pitch -= amount;
			m_Pitch = glm::clamp(m_Pitch, m_MinMaxPitch.x, m_MinMaxPitch.y);
			m_Position = m_LookAt - 
				glm::normalize(glm::vec3(
				cosf(m_Pitch) * cosf(m_Yaw),
				sinf(m_Pitch),
				cosf(m_Pitch) * sinf(m_Yaw)))
				* distanceToLookAt;
			break;
		}
	}
}

void Camera::OffsetYaw(float amount) noexcept
{
	m_Yaw += amount;
	m_IsDirty = true;
	m_InverseIsDirty = true;
}

void Camera::OffsetPitch(float amount) noexcept
{
	m_Pitch += amount;
	m_IsDirty = true;
	m_InverseIsDirty = true;

	if (m_Pitch > m_MinMaxPitch.y)
	{
		m_Pitch = m_MinMaxPitch.y;
	}
	else if (m_Pitch < m_MinMaxPitch.x)
	{
		m_Pitch = m_MinMaxPitch.x;
	}
}

void Camera::InvertPitch() noexcept
{
	m_Pitch = -m_Pitch;
	m_IsDirty = true;
	m_InverseIsDirty = true;
}

void Camera::SetPos(const glm::vec3& pos) noexcept
{
	m_Position = pos;
	m_IsDirty = true;
	m_InverseIsDirty = true;

	m_Front = glm::normalize(m_Position - m_LookAt);
	m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);
	m_DistanceToLookAt = glm::length(m_LookAt - m_Position);
}

void Camera::SetLookAt(const glm::vec3& lookAt) noexcept
{
	m_LookAt = lookAt;
	m_IsDirty = true;
	m_InverseIsDirty = true;

	m_Front = glm::normalize(m_Position - m_LookAt);
	m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);
	m_DistanceToLookAt = glm::length(m_LookAt - m_Position);
}

void Camera::SetYaw(float yaw) noexcept
{
	m_Yaw = yaw;
	m_IsDirty = true;
	m_InverseIsDirty = true;
}

void Camera::SetPitch(float pitch) noexcept
{
	m_Pitch = pitch;
	m_IsDirty = true;
	m_InverseIsDirty = true;

	if (m_Pitch > m_MinMaxPitch.y)
	{
		m_Pitch = m_MinMaxPitch.y;
	}
	else if (m_Pitch < m_MinMaxPitch.x)
	{
		m_Pitch = m_MinMaxPitch.x;
	}
}

void Camera::SetMinPitch(float min) noexcept
{
	m_MinMaxPitch.x = min;
}

void Camera::SetMaxPitch(float max) noexcept
{
	m_MinMaxPitch.y = max;
}

void Camera::SetMinDistToLookAt(float min) noexcept
{
	m_MinMaxDistanceToLookAt.x = min;
}

void Camera::SetMaxDistToLookAt(float max) noexcept
{
	m_MinMaxDistanceToLookAt.y = max;
}

void Camera::SetMinXZMaxXZLookAt(float minX, float minZ, float maxX, float maxZ) noexcept
{
	m_MinXZMaxXZLookAt.x = minX;
	m_MinXZMaxXZLookAt.y = minZ;
	m_MinXZMaxXZLookAt.z = maxX;
	m_MinXZMaxXZLookAt.w = maxZ;
}

void Camera::CalcInverses()
{
	if (m_InverseIsDirty)
	{
		m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
		m_InverseCombinedMatrix = m_InverseViewMatrix * m_InverseProjectionMatrix;

		m_InverseIsDirty = false;
	}
}

void Camera::UpdateFromPitchYawInternal() noexcept
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		m_Front = glm::normalize(glm::vec3(
			cosf(m_Pitch) * cosf(m_Yaw),
			sinf(m_Pitch),
			cosf(m_Pitch) * sinf(m_Yaw)));
		m_LookAt = m_Position + m_Front;
		m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
		m_CombinedMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}

void Camera::UpdateFromLookAtInternal() noexcept
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		m_Front = glm::normalize(m_LookAt - m_Position);
		//m_Pitch = asinf(m_Front.y);
		//m_Yaw = atan2(m_Front.x, m_Front.z);
		m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);

		m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, m_WorldUp);
		m_CombinedMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
