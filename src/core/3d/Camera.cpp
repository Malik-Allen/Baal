// MIT License, Copyright (c) 2024 Malik Allen

#include "Camera.h"

namespace Baal
{
	namespace VK
	{
		Camera::Camera()
		{
			position = Vector3f(0.0f);
			orientation = Quatf(0.0f);

			fieldOfView = 45.0f;
			nearPlane = 0.5f;
			farPlane = 100.0f;
			aspectRatio = 16.0f / 9.0f;

			view =  Matrix4f::LookAt(position, Vector3f(0.0f, 0.0f, -3.0f), Vector3f::UpVector);
			projection = Matrix4f::PerspectiveMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
		}

		void Camera::Update()
		{
			view = Matrix4f::LookAt(position, Vector3f(0.0f, 0.0f, -3.0f), Vector3f::UpVector);
		}
	}
}