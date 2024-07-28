#include "Components/Camera.h"

namespace Laura 
{	
	Camera::Camera(float fov, float aspectRatio)
		: fov(fov), aspectRatio(aspectRatio), transform()
	{
		// since we transform the size of the screen in the compute shader to "normalized device coordinates" or NDC for short (-1, 1) 
		// half of the screen width is 1. Therefore (screen width / 2) / tan(FOV in radians / 2) can be simplified to 1 / tan(FOV_rad / 2)
		focalLength = 1 / tan(fov / 2);
	}
}



