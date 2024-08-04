#include <GL/glew.h>
#include "platform/OpenGL/OpenGLComputeShader.h"
#include "platform/OpenGL/OpenGLdebugFuncs.h"

namespace Laura {

	OpenGLComputeShader::OpenGLComputeShader(const std::string& filepath, const glm::uvec3& workGroupSizes)
		: m_Filepath(filepath), m_WorkGroupSizes(workGroupSizes), m_ID(0)
	{
		CreateShader();
	}

	OpenGLComputeShader::~OpenGLComputeShader()
	{
		GLCall(glDeleteProgram(m_ID));
	}

	void OpenGLComputeShader::Bind()
	{
		GLCall(glUseProgram(m_ID));
	}

	void OpenGLComputeShader::Unbind()
	{
		GLCall(glUseProgram(0));
	}

	void OpenGLComputeShader::Dispatch()
	{
		glDispatchCompute(m_WorkGroupSizes.x, m_WorkGroupSizes.y, m_WorkGroupSizes.z);
		GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS))
	}

	std::string OpenGLComputeShader::ParseShaderFile()
	{
		std::ifstream stream(m_Filepath);
		std::stringstream ss;
		std::string line;

		while (getline(stream, line))
		{
			ss << line << '\n';
		}

		std::string source = ss.str();
		return source;
	}

	void OpenGLComputeShader::CreateShader()
	{
		std::string computeShaderSource = ParseShaderFile();
		const char* src = &computeShaderSource[0];
		uint32_t computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

		GLCall(glShaderSource(computeShaderID, 1, &src, NULL));
		GLCall(glCompileShader(computeShaderID));

		int result;
		GLCall(glGetShaderiv(computeShaderID, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(computeShaderID, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(computeShaderID, length, &length, message));
			LR_CORE_CRITICAL("[ERROR] Compute Shader compilaion error:");
			LR_CORE_CRITICAL(message);
			GLCall(glDeleteShader(computeShaderID));
			m_ID = 0;
			return;
		}

		m_ID = glCreateProgram(); // is m_RendererID
		GLCall(glAttachShader(m_ID, computeShaderID));
		GLCall(glLinkProgram(m_ID));
		GLCall(glValidateProgram(m_ID));
		GLCall(glDeleteShader(computeShaderID));
	}

}