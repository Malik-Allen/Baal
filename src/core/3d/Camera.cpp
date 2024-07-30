// MIT License, Copyright (c) 2024 Malik Allen

#include "Camera.h"

namespace Baal
{
	namespace VK
	{
		Camera::Camera()
		{
			position = Vector3f(0.0f, 10.0f, -9.0f);
			orientation = Quatf(0.0f);

			fieldOfView = 45.0f;
			nearPlane = 0.5f;
			farPlane = 100.0f;
			aspectRatio = 800.0f / 600.0f;

			matrices.view =  Matrix4f::LookAt(position, Vector3f(0.0f, 0.0f, 0.0f), Vector3f::UpVector);
			matrices.proj = Matrix4f::PerspectiveMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
			matrices.proj = matrices.proj * Matrix4f::Scale(Vector3f(1.0f, -1.0f, 1.0f)); // Flip y-axis to render in vulkan, up-side up
		}

		void Camera::Update()
		{
			matrices.view = Matrix4f::LookAt(position, Vector3f(0.0f, 0.0f, 0.0f), Vector3f::UpVector);
		}
	}
}