#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "entt/entt.hpp"
#include "Project/Scene/Entity.h"
#include "Project/Scene/Components.h"

#define SCENE_FILE_EXTENSION ".lrscn"

namespace Laura
{

	class Scene {
	public:
		Scene(std::string name = "NewScene")
			: m_SceneName(std::move(name)) {
			m_Registry = new entt::registry();
		}

		~Scene() {
			delete m_Registry;
		}

		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

		inline entt::registry* GetRegistry() const { return m_Registry; }
		inline LR_GUID GetSkyboxGUID() const { return m_SkyboxGUID; }
		inline void SetSkyboxGUID(LR_GUID guid) { m_SkyboxGUID = guid; }
		
		// guid set in constructor or updated when deserializing
		inline LR_GUID GetGUID() const { return m_SceneGUID; }

		inline const std::string& GetName() const { return m_SceneName; }
		inline void SetName(std::string name) { m_SceneName = std::move(name); }

	private:
		LR_GUID m_SceneGUID;
		std::string m_SceneName;

		LR_GUID m_SkyboxGUID;
		entt::registry* m_Registry;
	};
}