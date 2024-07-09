#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "lrpch.h"
#include "renderer/ITexture.h"

class OpenGLTexture: public ITexture
{
public:
	OpenGLTexture(const std::string& filepath, uint16_t bindingPoint);
	OpenGLTexture(int width, int height, uint16_t channels, uint16_t bindingPoint);
	virtual ~OpenGLTexture() override;

	virtual void Bind() override;
	virtual void Unbind() override;
	virtual void ChangeBindingPoint(uint16_t bindingPoint) override;

	inline virtual int GetWidth()  override { return m_Width; }
	inline virtual int GetHeight() override { return m_Height; }
	inline virtual uint16_t GetID()     override { return m_RendererID; }

private:
	uint16_t m_RendererID, m_BindingPoint, m_Channels;
	int m_Width, m_Height;
	std::string m_Filepath;
};

#endif // OPENGL_TEXTURE_H