#include <EnginePch.h>
#include <Graphics\Camera.h>

Camera::Camera(const glm::vec3& pos, float pitch, float yaw, const glm::vec3& upVector) noexcept
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
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, m_WorldUp);

	CalcInverses();
	m_IsDirty = false;
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& upVector) noexcept
{
	m_InverseIsDirty = true; 
	m_Position = pos;
	m_LookAt = lookAt;
	m_Front = glm::normalize(m_LookAt - m_Position);
	m_Up = glm::cross(glm::cross(m_Front, m_WorldUp), m_Front);
	m_WorldUp = upVector;
	m_Pitch = asinf(m_Front.y);
	m_Yaw = atan2(m_Front.x, m_Front.z) - glm::half_pi<float>();
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

void Camera::MoveCartesian(CameraDirCartesian dir, float amount) noexcept
{
	m_IsDirty = true;
	m_InverseIsDirty = true;

	switch (dir)
	{
	case CameraDirCartesian::Forward:
		m_Position += m_Front * amount;
		break;

	case CameraDirCartesian::Backwards:
		m_Position -= m_Front * amount;
		break;

	case CameraDirCartesian::Left:
		m_Position -= glm::normalize(glm::cross(m_Front, m_WorldUp)) * amount;
		break;

	case CameraDirCartesian::Right:
		m_Position += glm::normalize(glm::cross(m_Front, m_WorldUp)) * amount;
		break;

	case CameraDirCartesian::Up:
		m_Position -= glm::normalize(glm::cross(m_Front, glm::cross(m_Front, m_WorldUp))) * amount;
		break;

	case CameraDirCartesian::Down:
		m_Position += glm::normalize(glm::cross(m_Front, glm::cross(m_Front, m_WorldUp))) * amount;
		break;
	}
}

void Camera::MovePosPolar(CameraPosPolar dir, float amount) noexcept
{
	m_IsDirty = true;
	m_InverseIsDirty = true;

	switch (dir)
	{
		case CameraPosPolar::ZoomIn:
		{
			float distanceToLookAt = glm::length(m_LookAt - m_Position);
			if (distanceToLookAt > 0.1f)
			{
				m_Position += m_Front * amount;
			}
			break;
		}

		case CameraPosPolar::ZoomOut:
		{
			m_Position -= m_Front * amount;
			break;
		}

		case CameraPosPolar::RotateLeft:
		{
			float distanceToLookAt = glm::length(glm::vec3(m_LookAt.x - m_Position.x, 0.0f, m_LookAt.z - m_Position.z));
			m_Yaw += amount;
			m_Position.x = m_LookAt.x - cosf(m_Yaw) * distanceToLookAt;
			m_Position.z = m_LookAt.z - sinf(m_Yaw) * distanceToLookAt;
			break;
		}

		case CameraPosPolar::RotateRight:
		{
			float distanceToLookAt = glm::length(glm::vec3(m_LookAt.x - m_Position.x, 0.0f, m_LookAt.z - m_Position.z));
			m_Yaw -= amount;
			m_Position.x = m_LookAt.x - cosf(m_Yaw) * distanceToLookAt;
			m_Position.z = m_LookAt.z - sinf(m_Yaw) * distanceToLookAt;
			break;
		}

		case CameraPosPolar::RotateUp:
		{
			float distanceToLookAt = glm::length(m_LookAt - m_Position);
			m_Pitch -= amount;
			m_Pitch = glm::clamp(m_Pitch, -1.55334303f, 1.55334303f);
			m_Position = m_LookAt - 
				glm::normalize(glm::vec3(
				cosf(m_Pitch) * cosf(m_Yaw),
				sinf(m_Pitch),
				cosf(m_Pitch) * sinf(m_Yaw)))
				* distanceToLookAt;
			break;
		}

		case CameraPosPolar::RotateDown:
		{
			float distanceToLookAt = glm::length(m_LookAt - m_Position);
			m_Pitch += amount;
			m_Pitch = glm::clamp(m_Pitch, -1.55334303f, 1.55334303f);
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

void Camera::MoveLookAtAndPosPolar(CameraDirCartesian dir, float amount) noexcept
{
	m_IsDirty = true;
	m_InverseIsDirty = true;

	switch (dir)
	{
		case CameraDirCartesian::Forward:
		{
			glm::vec3 forward = glm::vec3(m_Front.x, 0.0f, m_Front.z);
			m_LookAt += forward * amount;
			m_Position += forward * amount;
			break;
		}

		case CameraDirCartesian::Backwards:
		{
			glm::vec3 forward = glm::vec3(m_Front.x, 0.0f, m_Front.z);
			m_LookAt -= forward * amount;
			m_Position -= forward * amount;
			break;
		}

		case CameraDirCartesian::Left:
		{
			glm::vec3 right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_LookAt -= right * amount;
			m_Position -= right * amount;
			break;
		}

		case CameraDirCartesian::Right:
		{
			glm::vec3 right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_LookAt += right * amount;
			m_Position += right * amount;
			break;
		}

		case CameraDirCartesian::Up:
		{
			m_LookAt += m_WorldUp * amount;
			m_Position += m_WorldUp * amount;
			break;
		}

		case CameraDirCartesian::Down:
		{
			m_LookAt -= m_WorldUp * amount;
			m_Position -= m_WorldUp * amount;
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

	if (m_Pitch > 1.55334303f)
	{
		m_Pitch = 1.55334303f;
	}
	else if (m_Pitch < -1.55334303f)
	{
		m_Pitch = -1.55334303f;
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
}

void Camera::SetLookAt(const glm::vec3& lookAt) noexcept
{
	m_LookAt = lookAt;
	m_IsDirty = true;
	m_InverseIsDirty = true;
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

	if (m_Pitch > 1.55334303f)
	{
		m_Pitch = 1.55334303f;
	}
	else if (m_Pitch < -1.55334303f)
	{
		m_Pitch = -1.55334303f;
	}
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
