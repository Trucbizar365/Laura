#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "lrpch.h"

class ITexture
{
public:
	static std::shared_ptr<ITexture> Create(const std::string& filepath, uint16_t bindingPoint);
	static std::shared_ptr<ITexture> Create(int width, int height, int channels, uint16_t bindingPoint);
	
	virtual ~ITexture() = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void ChangeBindingPoint(uint16_t bindingPoint) = 0;

	/// GETTERS ///
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetID() = 0;
};

#endif // ITEXTURE_H