#include "Project/Scene/SceneManager.h"

namespace Laura 
{

    LR_GUID SceneManager::NewScene(const std::string& name) {
        auto scene = std::make_shared<Scene>(name);
        m_Scenes[scene->GetGUID()] = scene;
        return scene->GetGUID();
    }

    void SceneManager::RemoveScene(LR_GUID guid) {
        if (m_ActiveSceneGUID == guid)  { m_ActiveSceneGUID = LR_GUID::INVALID; }
        if (m_BootSceneGUID == guid)    { m_BootSceneGUID   = LR_GUID::INVALID; }
        m_Scenes.erase(guid);
    }

    bool SceneManager::SetActiveScene(LR_GUID guid) {
        if (guid == LR_GUID::INVALID) {
            return false;
        }
        auto it = m_Scenes.find(guid); 
        if (it == m_Scenes.end()) {
            return false;
        }
        m_ActiveSceneGUID = guid; 
        return true;
    }

    std::shared_ptr<Scene> SceneManager::GetActiveScene() const {
        if (m_ActiveSceneGUID == LR_GUID::INVALID){
            return nullptr;
        }
        if (auto it = m_Scenes.find(m_ActiveSceneGUID); it != m_Scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool SceneManager::SetBootScene(LR_GUID guid) {
        if (m_Scenes.find(guid) == m_Scenes.end()) {
            return false;
        }
        m_BootSceneGUID = guid;
        return true;
    }

    std::shared_ptr<Scene> SceneManager::GetBootScene() const {
        if (m_BootSceneGUID == LR_GUID::INVALID) {
            return nullptr;
        }
        if (auto it = m_Scenes.find(m_BootSceneGUID); it != m_Scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool SceneManager::SetBootSceneAsActiveScene() {
        return SetActiveScene(m_BootSceneGUID);
    }

    bool SceneManager::Serialize(const std::filesystem::path& projectRoot) const {
        bool successAll = true;
        for (const auto& [guid, scene] : m_Scenes) {
            bool success = scene->Serialize(projectRoot);
            if (!success) { successAll = false; }
        }
        return successAll;
    }

    bool SceneManager::Deserialize(const std::filesystem::path& projectRoot) {
        bool successAll = true;
        for (const auto& scenepath : FindFilesInFolder(projectRoot, SCENE_FILE_EXTENSION)) {
            auto scene = std::make_shared<Scene>();
            bool success = scene->Deserialize(scenepath);
            if (!success) { 
                successAll = false; 
                continue;
            }
            m_Scenes[scene->GetGUID()] = scene;
        }
        return successAll;
    }

    static std::vector<std::filesystem::path> SceneManager::FindFilesInFolder(
            const std::filesystem::path& folder, 
            const std::string& extension) {

        std::vector<std::filesystem::path> result;
		if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
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
