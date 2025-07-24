#pragma once

#include "entt/entt.hpp"
#include "Scene/Entity.h"
#include "Scene/Components.h"

namespace Laura
{

	class Scene {
	public:
		Scene();
		~Scene();
		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle entity);

		void OnStart();
		void OnUpdate();
		void OnShutdown();

		inline entt::registry* GetRegistry() const { return m_Registry; }

		inline LR_GUID GetSkyboxGUID() const { return m_SkyboxGUID; }
		inline void SetSkyboxGUID(LR_GUID guid) { m_SkyboxGUID = guid; }

	private:
		LR_GUID m_SkyboxGUID;
		entt::registry* m_Registry;
	};
}