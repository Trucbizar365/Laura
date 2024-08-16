#pragma once

#include "lrpch.h"

namespace Laura
{
    struct LoadedTexture
    {
        unsigned char* data;
        int width, height, channels;
    };
    
    namespace TextureLoader
    {
        /// Desired Channels ///
        // Load the image with its original number of channels.
        LoadedTexture loadTexture(const std::string& filepath);

        /// Desired Channels ///
        // 1 : Convert to grayscale(single channel).
        // 2 : Convert to grayscale with alpha(two channels).
        // 3 : Convert to RGB(three channels).
        // 4 : Convert to RGBA(four channels).
        LoadedTexture loadTexture(const std::string& filepath, const int desired_channels);

        void freeTexture(LoadedTexture& texture_data);
    }
}