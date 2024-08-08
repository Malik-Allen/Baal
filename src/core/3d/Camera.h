// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_CAMERA_H
#define BAAL_VK_CAMERA_H

#include <Mjolnir.h>
#include <memory>

namespace Baal
{
	namespace VK
	{
		class Buffer;

		enum class AspectRatio
		{
			RATIO_4_3 = 0,
			RATIO_16_9,
			RATIO_16_10,
			RATIO_21_9,
			RATIO_1_1,
			RATIO_3_2,
			RATIO_5_4,
			RATIO_2_35_1,
			RATIO_2_39_1,
			CUSTOM_LOCKED,
			CUSTOM_UNLOCKED
		};

		struct CameraMatrices
		{
			Matrix4f model;
			Matrix4f view;
			Matrix4f proj;
		};

		class Camera
		{
		public:
			explicit Camera(const float FOV = 45.0f, const AspectRatio _aspectRatio = AspectRatio::RATIO_4_3, const uint32_t width = 0, const uint32_t height = 0);
			Camera(const Camera&) = delete;
			Camera(Camera&&) = delete;

			~Camera() = default;

			Camera& operator=(const Camera&) = delete;
			Camera& operator = (Camera&&) = delete;

			Matrix4f GetViewMatrix() const { return matrices.view; }
			Matrix4f GetProjectionMatrix() const { return matrices.proj; }
			CameraMatrices& GetMatrices() { return matrices; }
			Transform GetTransform() const { return transform; }

			void SetPosition(Vector3f position);
			void SetRotation(Quatf orientation);
			void SetAspectRatio(const AspectRatio _aspectRatio = AspectRatio::RATIO_4_3, const uint32_t width = 0, const uint32_t height = 0);
			void SetFOV(const float FOV);
			bool IsAspectRatioDynamic() const;

		private:
			Transform transform;

			float fieldOfView;
			AspectRatio aspectRatio;
			float ratio;
			float nearPlane;
			float farPlane;

			CameraMatrices matrices;

			void UpdateViewMatrix();
			void UpdateProjectionMatrix();
		};

		struct RenderCameraResources
		{
			std::unique_ptr<Camera> camera;
			std::unique_ptr<Buffer> uniformBuffer;
		};
	}
}

#endif // !BAAL_VK_CAMERA_H