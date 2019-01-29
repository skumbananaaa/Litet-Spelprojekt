#pragma once

#include <EnginePch.h>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

enum CameraDirCartesian : uint8
{
	Forward,
	Backwards,
	Left,
	Right,
	Up,
	Down
};

enum CameraPosPolar : uint8
{
	ZoomIn,
	ZoomOut,
	RotateLeft,
	RotateRight,
	RotateUp,
	RotateDown
};

class API Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(0.0f), float pitch = 0.0f, float yaw = 0.0f) noexcept;
	Camera(const glm::vec3& pos, const glm::vec3& lookAt) noexcept;
	~Camera();

	void UpdateFromPitchYaw() noexcept;
	void UpdateFromLookAt() noexcept;
	void UpdateFromPitchYawNoInverse() noexcept;
	void UpdateFromLookAtNoInverse() noexcept;

	//void SetProjectionMatrix(const glm::mat4& matrix) noexcept;

	void CreatePerspective(float fovRad, float aspectWihe, float nearPlane, float farPlane) noexcept;

	void MoveCartesian(CameraDirCartesian dir, float amount) noexcept;
	void MovePosPolar(CameraPosPolar dir, float amount) noexcept;
	void MoveLookAtAndPosPolar(CameraDirCartesian dir, float amount) noexcept;
	void OffsetYaw(float amount) noexcept;
	void OffsetPitch(float amount) noexcept;
	void InvertPitch() noexcept;

	void SetPos(const glm::vec3& pos) noexcept;
	void SetLookAt(const glm::vec3& lookAt) noexcept;

	void SetYaw(float yaw) noexcept;
	void SetPitch(float pitch) noexcept;

	const glm::mat4& GetViewMatrix() const noexcept;
	const glm::mat4& GetProjectionMatrix() const noexcept;
	const glm::mat4& GetCombinedMatrix() const noexcept;
	const glm::mat4& GetInverseViewMatrix() const noexcept;
	const glm::mat4& GetInverseProjectionMatrix() const noexcept;
	const glm::mat4& GetInverseCombinedMatrix() const noexcept;

	const glm::vec3& GetPosition() const noexcept;
	const glm::vec3& GetLookAt() const noexcept;
	const glm::vec3& GetFront() const noexcept;
	const glm::vec3& GetUp() const noexcept;

	float GetYaw() const noexcept;
	float GetPitch() const noexcept;
	float GetFarPlane() const noexcept;
	float GetNearPlane() const noexcept;

	void CopyShaderDataToArray(float* const arr, uint32 startIndex) const noexcept;

private:
	void CalcInverses();
	void UpdateFromPitchYawInternal() noexcept;
	void UpdateFromLookAtInternal() noexcept;

private:
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_CombinedMatrix;
	glm::mat4 m_InverseViewMatrix;
	glm::mat4 m_InverseProjectionMatrix;
	glm::mat4 m_InverseCombinedMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_LookAt;
	glm::vec3 m_Front;
	glm::vec3 m_Up;

	float m_Yaw;
	float m_Pitch;
	float m_Far;
	float m_Near;

	bool m_IsDirty;
	bool m_InverseIsDirty;
};

inline const glm::mat4& Camera::GetViewMatrix() const noexcept
{
	return m_ViewMatrix;
}

inline const glm::mat4& Camera::GetProjectionMatrix() const noexcept
{
	return m_ProjectionMatrix;
}

inline const glm::mat4& Camera::GetCombinedMatrix() const noexcept
{
	return m_CombinedMatrix;
}

inline const glm::mat4& Camera::GetInverseViewMatrix() const noexcept
{
	return m_InverseViewMatrix;
}

inline const glm::mat4& Camera::GetInverseProjectionMatrix() const noexcept
{
	return m_InverseProjectionMatrix;
}

inline const glm::mat4& Camera::GetInverseCombinedMatrix() const noexcept
{
	return m_InverseCombinedMatrix;
}

inline const glm::vec3& Camera::GetPosition() const noexcept
{
	return m_Position;
}

inline const glm::vec3& Camera::GetLookAt() const noexcept
{
	return m_LookAt;
}

inline const glm::vec3& Camera::GetFront() const noexcept
{
	return m_Front;
}

inline const glm::vec3& Camera::GetUp() const noexcept
{
	return m_Up;
}

inline float Camera::GetYaw() const noexcept
{
	return m_Yaw;
}

inline float Camera::GetPitch() const noexcept
{
	return m_Pitch;
}

inline float Camera::GetFarPlane() const noexcept
{
	return m_Far;
}

inline float Camera::GetNearPlane() const noexcept
{
	return m_Near;
}
