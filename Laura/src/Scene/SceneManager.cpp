#include "Scene/SceneManager.h"

namespace Laura
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	std::shared_ptr<RenderableScene> SceneManager::ParseSceneForRendering(std::shared_ptr<Scene> scene)
	{
		std::shared_ptr<RenderableScene> renderableScene = std::make_shared<RenderableScene>();
		renderableScene->skyboxID = scene->skyboxID;

		// If the meshes have changed since the last frame
		renderableScene->meshesDirty = false;

		std::unordered_set<GUID> deleted_meshIDs = m_ActiveMeshIDs;

		for (auto entity : scene->Get()->view<TransformComponent>())
		{
			Entity e(entity, scene->Get());
			if (e.HasComponent<CameraComponent>() && e.HasComponent<TransformComponent>())
			{
				if (e.GetComponent<CameraComponent>().isMain)
				{
					renderableScene->mainCamera = e;
				}
			}

			if (e.HasComponent<MeshComponent>() && e.HasComponent<TransformComponent>())
			{
				GUID id = e.GetComponent<GUIDComponent>().id;
				if (m_ActiveMeshIDs.find(id) == m_ActiveMeshIDs.end())
				{
					m_ActiveMeshIDs.insert(id);
					renderableScene->meshesDirty = true;
				}
				else
				{
					deleted_meshIDs.erase(id); // Mesh has not been deleted so remove it from the set
				}
				renderableScene->models.push_back(e);
			}
		}

		for (GUID id : deleted_meshIDs)
		{
			m_ActiveMeshIDs.erase(id);
			renderableScene->meshesDirty = true;
		}
		return renderableScene;
	}
}