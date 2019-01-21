#pragma once

#include <EnginePch.h>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

enum CameraDir : char
{
	Forward,
	Backwards,
	Left,
	Right,
	Up,
	Down
};

struct DataToShader
{
	float CameraCombined[16];
	float CameraPosition[3];
};

class API Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(0.0f), float pitch = 0.0f, float yaw = 0.0f) noexcept;
	~Camera();

	void Update() noexcept;

	void SetProjectionMatrix(const glm::mat4& matrix) noexcept;

	void Move(CameraDir dir, float amount) noexcept;
	void OffsetYaw(float amount) noexcept;
	void OffsetPitch(float amount) noexcept;

	void SetPos(const glm::vec3& pos) noexcept;
	void SetFront(const glm::vec3& front) noexcept;

	void SetYaw(float yaw) noexcept;
	void SetPitch(float pitch) noexcept;

	const glm::mat4& GetViewMatrix() const noexcept;
	const glm::mat4& GetProjectionMatrix() const noexcept;
	const glm::mat4& GetCombinedMatrix() const noexcept;

	const glm::vec3& GetPos() const noexcept;
	const glm::vec3& GetFront() const noexcept;

	float GetYaw() const noexcept;
	float GetPitch() const noexcept;

	const DataToShader& GetDataToShader() const noexcept;

private:
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_CombinedMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_Front;

	float m_Yaw;
	float m_Pitch;

	bool m_IsDirty;

	DataToShader m_DataToShader;
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

inline const glm::vec3& Camera::GetPos() const noexcept
{
	return m_Position;
}

inline const glm::vec3& Camera::GetFront() const noexcept
{
	return m_Front;
}

inline float Camera::GetYaw() const noexcept
{
	return m_Yaw;
}

inline float Camera::GetPitch() const noexcept
{
	return m_Pitch;
}

inline const DataToShader& Camera::GetDataToShader() const noexcept
{
	return m_DataToShader;
}
