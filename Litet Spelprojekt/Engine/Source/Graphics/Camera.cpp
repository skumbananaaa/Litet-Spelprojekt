#include <EnginePch.h>
#include <Graphics\Camera.h>

Camera::Camera(const glm::vec3& pos, float pitch, float yaw) noexcept
{
	m_Position = pos;
	m_Pitch = pitch;
	m_Yaw = yaw;
	m_Front = glm::normalize(glm::vec3(
		cosf(m_Pitch) * cosf(m_Yaw),
		sinf(m_Pitch),
		cosf(m_Pitch) * sinf(m_Yaw)));
	m_LookAt = m_Position + m_Front;
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, UP_VECTOR);
	m_IsDirty = false;
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& lookAt) noexcept
{
	m_Position = pos;
	m_LookAt = lookAt;
	m_Front = glm::normalize(m_LookAt - m_Position);
	m_Pitch = asinf(m_Front.y);
	m_Yaw = atan2(m_Front.x, m_Front.z) - glm::half_pi<float>();
	m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, UP_VECTOR);
	m_IsDirty = false;
}

Camera::~Camera()
{
}

void Camera::UpdateFromPitchYaw() noexcept
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		m_Front = glm::normalize(glm::vec3(
			cosf(m_Pitch) * cosf(m_Yaw),
			sinf(m_Pitch),
			cosf(m_Pitch) * sinf(m_Yaw)));
		m_LookAt = m_Position + m_Front;

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, UP_VECTOR);
		m_CombinedMatrix = m_ProjectionMatrix * m_ViewMatrix;

		m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
		m_InverseCombinedMatrix = glm::inverse(m_CombinedMatrix);
		m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
	}
}

void Camera::UpdateFromLookAt() noexcept
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		m_Front = glm::normalize(m_LookAt - m_Position);
		//m_Pitch = asinf(m_Front.y);
		//m_Yaw = atan2(m_Front.x, m_Front.z);

		m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, UP_VECTOR);
		m_CombinedMatrix = m_ProjectionMatrix * m_ViewMatrix;

		m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
		m_InverseCombinedMatrix = glm::inverse(m_CombinedMatrix);
		m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
	}
}

/*void Camera::SetProjectionMatrix(const glm::mat4& matrix) noexcept
{
	m_ProjectionMatrix = matrix;
	m_IsDirty = true;
}*/

void Camera::CreatePerspective(float fovRad, float aspectWihe, float nearPlane, float farPlane) noexcept
{
	m_ProjectionMatrix = glm::perspective(fovRad, aspectWihe, nearPlane, farPlane);
	m_Near = nearPlane;
	m_Far = farPlane;

	m_IsDirty = true;
}

void Camera::MoveCartesian(CameraDirCartesian dir, float amount) noexcept
{
	m_IsDirty = true;

	switch (dir)
	{
	case CameraDirCartesian::Forward:
		m_Position += m_Front * amount;
		break;

	case CameraDirCartesian::Backwards:
		m_Position -= m_Front * amount;
		break;

	case CameraDirCartesian::Left:
		m_Position -= glm::normalize(glm::cross(m_Front, UP_VECTOR)) * amount;
		break;

	case CameraDirCartesian::Right:
		m_Position += glm::normalize(glm::cross(m_Front, UP_VECTOR)) * amount;
		break;

	case CameraDirCartesian::Up:
		m_Position -= glm::normalize(glm::cross(m_Front, glm::cross(m_Front, UP_VECTOR))) * amount;
		break;

	case CameraDirCartesian::Down:
		m_Position += glm::normalize(glm::cross(m_Front, glm::cross(m_Front, UP_VECTOR))) * amount;
		break;
	}
}

void Camera::MovePosPolar(CameraPosPolar dir, float amount) noexcept
{
	m_IsDirty = true;


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
			glm::vec3 right = glm::normalize(glm::cross(m_Front, UP_VECTOR));
			m_LookAt -= right * amount;
			m_Position -= right * amount;
			break;
		}

		case CameraDirCartesian::Right:
		{
			glm::vec3 right = glm::normalize(glm::cross(m_Front, UP_VECTOR));
			m_LookAt += right * amount;
			m_Position += right * amount;
			break;
		}

		case CameraDirCartesian::Up:
		{
			m_LookAt += UP_VECTOR * amount;
			m_Position += UP_VECTOR * amount;
			break;
		}

		case CameraDirCartesian::Down:
		{
			m_LookAt -= UP_VECTOR * amount;
			m_Position -= UP_VECTOR * amount;
			break;
		}
	}
}

void Camera::OffsetYaw(float amount) noexcept
{
	m_Yaw += amount;
	m_IsDirty = true;
}

void Camera::OffsetPitch(float amount) noexcept
{
	m_Pitch += amount;
	m_IsDirty = true;

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
}

void Camera::SetPos(const glm::vec3& pos) noexcept
{
	m_Position = pos;
	m_IsDirty = true;
}

void Camera::SetLookAt(const glm::vec3& lookAt) noexcept
{
	m_LookAt = lookAt;
	m_IsDirty = true;
}

void Camera::SetYaw(float yaw) noexcept
{
	m_Yaw = yaw;
	m_IsDirty = true;
}

void Camera::SetPitch(float pitch) noexcept
{
	m_Pitch = pitch;
	m_IsDirty = true;

	if (m_Pitch > 1.55334303f)
	{
		m_Pitch = 1.55334303f;
	}
	else if (m_Pitch < -1.55334303f)
	{
		m_Pitch = -1.55334303f;
	}
}

void Camera::CopyShaderDataToArray(float* const arr, uint32 startIndex) const noexcept
{
	const float* pSource = (const float*)glm::value_ptr(m_CombinedMatrix);
	for (int i = 0; i < 16; i++)
	{
		arr[startIndex + i] = pSource[i];
	}

	arr[startIndex + 16] = m_Position.x;
	arr[startIndex + 17] = m_Position.y;
	arr[startIndex + 18] = m_Position.z;
}
