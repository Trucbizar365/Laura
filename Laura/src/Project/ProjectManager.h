#pragma once 

#include "lrpch.h"
#include <filesystem>
#include "Project/Scene/SceneManager.h"
#include "Project/Assets/AssetManager.h"

#define PROJECT_FILE_EXTENSION ".lrproj"

namespace Laura 
{

	class ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		void NewProject(std::filesystem::path projectFolderpath);
		bool OpenProject(std::filesystem::path projectFolderpath);
		bool SaveProject();
		void CloseProject();


		inline std::filesystem::path GetProjectFolderpath() const { return m_ProjectFolderpath; }
		inline std::string GetProjectFoldername() const { return m_ProjectFolderpath.filename().string(); }

		inline bool isProjectOpen() const { return !m_ProjectFolderpath.empty() }
		
		inline std::weak_ptr<SceneManager> GetSceneManager() { return m_SceneManager; }
		inline std::weak_ptr<AssetManager> GetAssetManager() { return m_AssetManager; }

	private:
		bool SerializeProjFile(const std::filesystem::path& projectFolderpath) const;
		bool DeserializeProjFile(const std::filesystem::path& projectFolderpath);

		std::filesystem::path m_ProjectFolderpath;
		LR_GUID m_BootSceneGuid = LR_GUID::INVALID;
		std::shared_ptr<SceneManager> m_SceneManager = nullptr;
		std::shared_ptr<AssetManager> m_AssetManager = nullptr;
	};
}