#pragma once
#include "lrpch.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Assets/TextureLoader.h"

namespace Laura
{
	struct RenderableScene
	{
		GUID skyboxID;
		Entity mainCamera;
		bool meshesDirty, materialsDirty, transformsDirty;
		std::vector<Entity> models;
	};

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		std::shared_ptr<RenderableScene> ParseSceneForRendering(std::shared_ptr<Scene> scene);
	private:
		std::unordered_set<GUID> m_ActiveMeshIDs;
	};
}