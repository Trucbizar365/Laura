#include "renderer/ITexture.h"
#include "renderer/Renderer.h"

#include "platform/OpenGL/shaders/OpenGLComputeShader.h"

std::shared_ptr<ITexture> ITexture::Create(const std::string& filepath, uint16_t bindingPoint)
{
	switch (Renderer::GetAPI())
	{
	case RenderingAPI::None: LR_CORE_CRITICAL("in ITexture::Create() - RendererAPI::None UNSUPPORTED"); return nullptr;
	case RenderingAPI::OpenGL: return std::make_shared<OpenGLTexture>(filepath, bindingPoint);
	}
}

std::shared_ptr<ITexture> ITexture::Create(int width, int height, int channels, uint16_t bindingPoint)
{
	switch (Renderer::GetAPI())
	{
	case RenderingAPI::None: LR_CORE_CRITICAL("in ITexture::Create() - RendererAPI::None UNSUPPORTED"); return nullptr;
	case RenderingAPI::OpenGL: return std::make_shared<OpenGLTexture>(int width, int height, int channels, uint16_t bindingPoint);
	}
}
