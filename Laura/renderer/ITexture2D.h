#ifndef ITEXTURE2D_H
#define ITEXTURE2D_H

#include "lrpch.h"

namespace Laura {

	class ITexture2D
	{
	public:
		static std::shared_ptr<ITexture2D> Create(unsigned char* data, int width, int height, int textureUnit);
		virtual ~ITexture2D() {}
		virtual void ChangeTextureUnit(int textureUnit) = 0;
		virtual int GetID() const = 0;
	};

}

#endif // ITEXTURE2D_H