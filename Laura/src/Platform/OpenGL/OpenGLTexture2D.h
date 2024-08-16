#pragma once

#include "lrpch.h"
#include "Renderer/ITexture2D.h"

namespace Laura {

	class OpenGLTexture2D : public ITexture2D
	{
	public:
		OpenGLTexture2D(unsigned char* data, int width, int height, int textureUnit);
		virtual ~OpenGLTexture2D() override;
		virtual void ChangeTextureUnit(int textureUnit) override;
		inline virtual int GetID() const override { return m_ID; }
	private:
		unsigned int m_ID;
		int m_TextureUnit;
	};

}