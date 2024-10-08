// MIT License, Copyright (c) 2024 Malik Allen

#include "Camera.h"

#include "../src/utility/DebugLog.h"

namespace Baal
{
	namespace VK
	{
        float CalcAspectRatio(AspectRatio ratio)
        {
            switch (ratio) {
            case AspectRatio::RATIO_4_3:
                return 4.0f / 3.0f;
            case AspectRatio::RATIO_16_9:
                return 16.0f / 9.0f;
            case AspectRatio::RATIO_16_10:
                return 16.0f / 10.0f;
            case AspectRatio::RATIO_21_9:
                return 21.0f / 9.0f;
            case AspectRatio::RATIO_1_1:
                return 1.0f / 1.0f;
            case AspectRatio::RATIO_3_2:
                return 3.0f / 2.0f;
            case AspectRatio::RATIO_5_4:
                return 5.0f / 4.0f;
            case AspectRatio::RATIO_2_35_1:
                return 2.35f / 1.0f;
            case AspectRatio::RATIO_2_39_1:
                return 2.39f / 1.0f;
            case AspectRatio::CUSTOM_LOCKED:
            case AspectRatio::CUSTOM_UNLOCKED:
                throw std::invalid_argument("Custom aspect ratio requires a specific value.");
            default:
                throw std::invalid_argument("Unknown aspect ratio.");
            }
        }

		Camera::Camera(const float FOV /*= 45.0f*/, const AspectRatio _aspectRatio /*= AspectRatio::RATIO_4_3*/, const uint32_t width /*= 0*/, const uint32_t height /*= 0*/)
		{
			transform = Transform();
			transform.SetPosition(Vector3f(0.0f, 0.0f, -10.0f));

            UpdateViewMatrix();


            nearPlane = 0.5f;
            farPlane = 100.0f;

			fieldOfView = FOV;
            SetAspectRatio(_aspectRatio, width, height);

            UpdateProjectionMatrix();
		}

        void Camera::SetPosition(Vector3f position)
        {
            transform.SetPosition(position);
            UpdateViewMatrix();
        }

        void Camera::SetRotation(Quatf orientation)
        {
            transform.SetRotation(orientation);
            UpdateViewMatrix();
        }

        void Camera::SetAspectRatio(const AspectRatio _aspectRatio, const uint32_t width /*= 0*/, const uint32_t height /*= 0*/)
        {
            aspectRatio = _aspectRatio;

            if (aspectRatio == AspectRatio::CUSTOM_LOCKED || aspectRatio == AspectRatio::CUSTOM_UNLOCKED)
            {
#ifdef _DEBUG
                if (height == 0)
                {
                    DEBUG_LOG(LOG::WARNING, "Dividing by zero in aspect ratio calculation! Please provide valid 'width' and 'height' values greater than zero.");
                    throw std::runtime_error("Warning! Dividing by zero in aspect ratio calculation!");
                }
#endif
                ratio = static_cast<float>(width) / static_cast<float>(height);
            }
            else
            {
                ratio = CalcAspectRatio(aspectRatio);
            }

            UpdateProjectionMatrix();
        }

        void Camera::SetFOV(const float FOV)
        {
            fieldOfView = FOV;
            UpdateProjectionMatrix();
        }

        bool Camera::IsAspectRatioDynamic() const
        {
            return aspectRatio == AspectRatio::CUSTOM_UNLOCKED;
        }

		void Camera::UpdateViewMatrix()
		{
			const Vector3f rotatedFowardVector = transform.GetRotation().RotateVector(Vector3f::ForwardVector);	// Rotate the forward vector to face the direction of the orientaion
			matrix.view = Matrix4f::LookAt(transform.GetPosition(), rotatedFowardVector + transform.GetPosition(), Vector3f::UpVector);

            const Vector3f pos = transform.GetPosition();
            matrix.pos = Vector4f(pos.x, pos.y, pos.z, 1.0f);
		}

        void Camera::UpdateProjectionMatrix()
        {
            matrix.proj = Matrix4f::PerspectiveMatrix(fieldOfView, ratio, nearPlane, farPlane);
            matrix.proj = matrix.proj * Matrix4f::Scale(Vector3f(-1.0f, -1.0f, 1.0f)); // We flip x and y axes to follow the right-hand rule of vulkan
        }
	}
}