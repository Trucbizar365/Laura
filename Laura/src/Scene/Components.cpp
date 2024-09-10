#include "Components.h"


namespace Laura 
{
	namespace TransformHandler
	{
		glm::vec3 GetTranslation(TransformComponent& transform)
		{
			return glm::vec3(transform.Transform[3]);
		}

		glm::vec3 GetScale(TransformComponent& transform)
		{
			glm::vec3 scale;
			scale.x = glm::length(glm::vec3(transform.Transform[0])); // Length of the X axis
			scale.y = glm::length(glm::vec3(transform.Transform[1])); // Length of the Y axis
			scale.z = glm::length(glm::vec3(transform.Transform[2])); // Length of the Z axis
			return scale;
		}

		glm::vec3 GetRotation(TransformComponent& transform)
		{
			glm::vec3 scale = GetScale(transform);

			// Normalize the matrix columns to remove the scale component
			glm::mat4 rotationMatrix = transform.Transform;

			rotationMatrix[0] /= scale.x;
			rotationMatrix[1] /= scale.y;
			rotationMatrix[2] /= scale.z;

			// Extract the Euler angles from the rotation matrix
			glm::vec3 rotation;
			rotation.x = atan2(rotationMatrix[1][2], rotationMatrix[2][2]);
			rotation.y = atan2(-rotationMatrix[0][2], glm::sqrt(rotationMatrix[1][2] * rotationMatrix[1][2] + rotationMatrix[2][2] * rotationMatrix[2][2]));
			rotation.z = atan2(rotationMatrix[0][1], rotationMatrix[0][0]);

			return glm::degrees(rotation); // Convert to degrees if necessary
		}

		void SetRotation(TransformComponent& transform, glm::vec3& angles)
		{
			glm::mat4 rotation{1.0f};
			
			rotation = glm::rotate(rotation, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
			rotation = glm::rotate(rotation, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
			rotation = glm::rotate(rotation, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));

			transform.Transform = rotation;
		}

		void SetTranslation(TransformComponent& transform, const glm::vec3& translation)
		{
			transform.Transform = glm::translate(glm::mat4(1.0f), translation);
		}

		void SetScale(TransformComponent& transform, const glm::vec3& scale)
		{
			transform = glm::scale(glm::mat4(1.0f), scale);
		}
	}

}