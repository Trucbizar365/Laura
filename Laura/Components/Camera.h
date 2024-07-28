#ifndef CAMERA_H
#define CAMERA_H

#include "lrpch.h"

#include "Components/TransformComponent.h"

namespace Laura 
{
	class Camera
	{
	public:
		Camera(float fov, float aspectRatio);
		
		float fov, aspectRatio, focalLength;
		TransformComponent transform;
	};
}

#endif // CAMERA_H