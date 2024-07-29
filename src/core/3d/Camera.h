// MIT License, Copyright (c) 2024 Malik Allen

#ifndef BAAL_VK_CAMERA_H
#define BAAL_VK_CAMERA_H

#include <Mjolnir.h>

namespace Baal
{
	namespace VK
	{
		// TODO: Add enum to lock aspect ratio and unlock aspect ratio
			// When locked, the window/viewport size will not be considered
			// When unlocked, the window aspect ratio will be used explcitly

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

			Matrix4f GetViewMatrix() const { return view; }
			Matrix4f GetProjectionMatrix() const { return projection; }

		private:
			Vector3f position;
			Quatf orientation;

			float fieldOfView;
			float nearPlane;
			float farPlane;
			float aspectRatio;

			Matrix4f view;
			Matrix4f projection;
		};
	}
}

#endif // !BAAL_VK_CAMERA_H