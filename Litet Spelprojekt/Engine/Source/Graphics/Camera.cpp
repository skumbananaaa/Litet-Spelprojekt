#include <EnginePch.h>
#include <Graphics\Camera.h>

Camera::Camera(const glm::vec3& pos, float pitch, float yaw) noexcept
{
	m_Position = pos;
	m_Pitch = pitch;
	m_Yaw = yaw;
	m_Front.x = cos(m_Pitch) * cos(m_Yaw);
	m_Front.y = sin(m_Pitch);
	m_Front.z = cos(m_Pitch) * sin(m_Yaw);
	m_Front = glm::normalize(m_Front);
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, UP_VECTOR);
	m_IsDirty = false;
}

Camera::~Camera()
{
}

void Camera::Update() noexcept
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		m_Front.x = cos(m_Pitch) * cos(m_Yaw);
		m_Front.y = sin(m_Pitch);
		m_Front.z = cos(m_Pitch) * sin(m_Yaw);
		m_Front = glm::normalize(m_Front);

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, UP_VECTOR);
		m_CombinedMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}

void Camera::SetProjectionMatrix(const glm::mat4& matrix) noexcept
{
	m_ProjectionMatrix = matrix;
	m_IsDirty = true;
}

void Camera::Move(CameraDir dir, float amount) noexcept
{
	m_IsDirty = true;

	switch (dir)
	{
	case CameraDir::Forward:
		m_Position += m_Front * amount;
		break;

	case CameraDir::Backwards:
		m_Position -= m_Front * amount;
		break;

	case CameraDir::Left:
		m_Position -= glm::normalize(glm::cross(m_Front, UP_VECTOR)) * amount;
		break;

	case CameraDir::Right:
		m_Position += glm::normalize(glm::cross(m_Front, UP_VECTOR)) * amount;
		break;

	case CameraDir::Up:
		m_Position -= glm::normalize(glm::cross(m_Front, glm::cross(m_Front, UP_VECTOR))) * amount;
		break;

	case CameraDir::Down:
		m_Position += glm::normalize(glm::cross(m_Front, glm::cross(m_Front, UP_VECTOR))) * amount;
		break;
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

void Camera::SetFront(const glm::vec3& front) noexcept
{
	m_Front = front;
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
