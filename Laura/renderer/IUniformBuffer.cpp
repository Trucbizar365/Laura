#include "IUniformBuffer.h"

#include "renderer/IRendererAPI.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"

namespace Laura 
{

	std::shared_ptr<IUniformBuffer> IUniformBuffer::Create(uint32_t size, uint32_t bindingPoint, BufferUsageType type)
	{
		switch (IRendererAPI::GetAPI())
		{
		case IRendererAPI::API::None: LR_CORE_CRITICAL("RendererAPI::None - UNSUPPORTED"); return nullptr;
		case IRendererAPI::API::OpenGL: return std::make_shared<OpenGLUniformBuffer>(size, bindingPoint, type);
		}
	}

}
