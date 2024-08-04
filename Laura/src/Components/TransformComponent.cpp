#include "TransformComponent.h"

namespace Laura
{

	TransformComponent::TransformComponent()
		: m_Position(0.0f, 0.0f, 0.0f), m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), m_Scale(1.0f, 1.0f, 1.0f)
	{
	}

	TransformComponent::~TransformComponent()
    {
    }

    void TransformComponent::setRotation(glm::vec3 eulerAngles)
    {
        glm::quat quatX = glm::angleAxis(glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat quatY = glm::angleAxis(glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat quatZ = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
        m_Rotation = quatZ * quatY * quatX;
    }

    void TransformComponent::setPosition(glm::vec3 position)
    {
        m_Position = position;
    }

    void TransformComponent::setScale(glm::vec3 scale)
    {
        m_Scale = scale;
    }

    glm::mat4 TransformComponent::toMatrix() const
    {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
        glm::mat4 rotationMatrix = glm::toMat4(m_Rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
}