#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "lrpch.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Laura 
{
	class TransformComponent
	{
	public:
		TransformComponent();
		~TransformComponent();	
		
		void setRotation(glm::vec3 eulerAngles);
		void setPosition(glm::vec3 position);
		void setScale(glm::vec3 scale);

		glm::mat4 toMatrix() const;
	
	private:
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;
	};
}

#endif // TRANSFORM_COMPONENT_H