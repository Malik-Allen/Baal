// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_CAMERA_H
#define BAAL_VK_CAMERA_H

#include <Mjolnir.h>
#include <memory>

namespace Baal
{
	namespace VK
	{
		// TODO: Add enum to lock aspect ratio and unlock aspect ratio
			// When locked, the window/viewport size will not be considered
			// When unlocked, the window aspect ratio will be used explcitly

		struct CameraMatrices
		{
			Matrix4f view;
			Matrix4f proj;
		};

		class Camera
		{
		public:
			Camera();
			Camera(const Camera&) = delete;
			Camera(Camera&&) = delete;

			~Camera() = default;

			Camera& operator=(const Camera&) = delete;
			Camera& operator = (Camera&&) = delete;

			void Update();

			Matrix4f GetViewMatrix() const { return matrices.view; }
			Matrix4f GetProjectionMatrix() const { return matrices.proj; }
			CameraMatrices& GetMatricies() { return matrices; }

		private:
			Vector3f position;
			Quatf orientation;

			float fieldOfView;
			float nearPlane;
			float farPlane;
			float aspectRatio;

			CameraMatrices matrices;
		};
	}
}

#endif // !BAAL_VK_CAMERA_H