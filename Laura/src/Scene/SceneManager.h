#pragma once
#include "lrpch.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include "Assets/TextureLoader.h"
#include "Renderer/BVH/BVHBuilder.h"
#include "Assets/AssetManager.h"

namespace Laura
{
	struct RenderableScene
	{
		std::shared_ptr<LoadedTexture> skybox;
		
		glm::mat4 cameraTransform;
		float cameraFocalLength;
	
		std::vector<int> meshMappings;
		std::vector<int> materialMappings;

		std::vector<glm::mat4> transforms;
		std::vector<std::shared_ptr<std::vector<Triangle>>> meshes;
		std::vector<std::shared_ptr<BVH::BVH_data>> BVHs;
		std::vector<std::shared_ptr<Material>> materials;

		bool skyboxDirty, meshesDirty, materialsDirty;

		bool isValid;
	};

	class SceneManager
	{
	public:
		inline SceneManager(std::shared_ptr<AssetManager> assetManager)
			: m_AssetManager(assetManager)
		{
		}
		~SceneManager() = default;
		std::shared_ptr<RenderableScene> ParseSceneForRendering(std::shared_ptr<Scene> scene);
	private:
		std::shared_ptr<AssetManager> m_AssetManager;

		GUID m_Cached_SkyboxGUID;
		std::vector<int> m_Cached_MeshMappings;
		std::vector<int> m_Cached_MaterialMappings;
	};
}