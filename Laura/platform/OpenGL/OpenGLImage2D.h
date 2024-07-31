#ifndef OPENGL_IMAGE2D_H
#define OPENGL_IMAGE2D_H

#include "lrpch.h"
#include <gl/glew.h>
#include "renderer/IImage2D.h"

namespace Laura 
{
	class OpenGLImage2D : public IImage2D
	{
	public:
		OpenGLImage2D(unsigned char* data, int width, int height, int imageUnit, Image2DType imageType);
		virtual ~OpenGLImage2D() override;
		virtual void ChangeImageUnit(int imageUnit) override;
		inline virtual int GetID() const override { return m_ID; }
	private:
		unsigned int m_ID;
		int m_ImageUnit;
		GLenum m_Image2DType;
	};
}

#endif // OPENGL_IMAGE2D_H
