#include "TextureLoader.h"

#include <stb_image/stb_image.h>

namespace Laura
{
	namespace TextureLoader
	{
		LoadedTexture loadTexture(const std::string& filepath)
		{
			return loadTexture(filepath, 0);
		}

		LoadedTexture loadTexture(const std::string& filepath, const int desired_channels)
		{
			LoadedTexture textureData;
			stbi_set_flip_vertically_on_load(1); // flip the image vertically to make sure the 0,0 coordinate is at the bottom left corner
			textureData.data = stbi_load(filepath.c_str(), &textureData.width, &textureData.height, &textureData.channels, desired_channels);
			if (!textureData.data)
			{
				LR_CORE_CRITICAL("loadTexture() failed to load the texture: {0}", filepath);
				return {};
			}
			return textureData;
		}

		void freeTexture(LoadedTexture& texture_data)
		{
			if (texture_data.data)
			{
				stbi_image_free(texture_data.data);
				texture_data.data = nullptr;
				texture_data.width = texture_data.height = texture_data.channels = 0;
			}
		}
	}
}