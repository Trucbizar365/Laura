#include "OpenGLShaderStorageBuffer.h"
#include <GL/glew.h>
#include "platform/OpenGL/OpenGLdebugFuncs.h"

namespace Laura
{

	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t size, uint32_t bindingPoint, BufferUsageType type)
		: m_ID(0), m_Size(size), m_BindingPoint(bindingPoint), m_UsageType(type)
	{
		GLCall(glGenBuffers(1, &m_ID));
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, nullptr, (m_UsageType == BufferUsageType::STATIC_DRAW) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_ID)); // binding the UBO to the binding point
	}

	void OpenGLShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
	}

	void OpenGLShaderStorageBuffer::Unbind()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLShaderStorageBuffer::AddData(uint32_t offset, uint32_t dataSize, const void* data)
	{
		GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, dataSize, data));
	}

	void OpenGLShaderStorageBuffer::SetBindingPoint(uint32_t bindingPoint)
	{
		m_BindingPoint = bindingPoint;
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_ID));
	}

}