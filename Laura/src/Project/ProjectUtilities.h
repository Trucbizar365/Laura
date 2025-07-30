#pragma once 

#include "lrpch.h"
#include "Core/Log.h"
#include <filesystem>

namespace Laura 
{
	
    inline std::vector<std::filesystem::path> FindFilesInFolder(const std::filesystem::path& folder, const std::string& extension) {
        std::vector<std::filesystem::path> result;
		if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
            LOG_ENGINE_WARN("Folder does not exists or is not a directory {0}", folder);
			return result;
		}
		for (const auto& entry : std::filesystem::directory_iterator(folder)) {
			if (entry.is_regular_file() && entry.path().extension() == extension) {
				result.push_back(entry.path());
			}
		}
		return result; 
    }

}