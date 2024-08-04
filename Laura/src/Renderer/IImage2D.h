#ifndef IIMAGE2D_H
#define IIMAGE2D_H

#include "lrpch.h"

namespace Laura {

	enum struct Image2DType
	{
		LR_READ,
		LR_WRITE,
		LR_READ_WRITE
	};

	class IImage2D
	{
	public:
		static std::shared_ptr<IImage2D> Create(unsigned char* data, int width, int height, int imageUnit, Image2DType imageType);
		virtual ~IImage2D() {}
		virtual void ChangeImageUnit(int imageUnit) = 0;
		virtual int GetID() const = 0;
	};
}

#endif // IIMAGE2D_H