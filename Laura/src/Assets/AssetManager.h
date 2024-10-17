#pragma once 

#include "lrpch.h"
#include "Geometry/Triangle.h"
#include "Renderer/BVH/BVHBuilder.h"

#include "Renderer/Material.h"
#include "Assets/TextureLoader.h" // LoadedTexture struct

#include "Core/GUID.h"


namespace Laura
{
	class AssetManager
	{
	public:
		GUID LoadTexture(const std::string& filepath, uint32_t channels);
		GUID LoadMesh(const std::string& filepath);
		uint32_t LoadMaterial(const std::string& filepath);

		void UnloadMesh(GUID id);
		void UnloadTexture(GUID id);
		void UnloadMaterial(uint32_t id);

		std::shared_ptr<std::vector<Triangle>> GetMesh(GUID id);
		std::shared_ptr<BVH::BVH_data> GetBVH(GUID id);
		std::shared_ptr<Material> GetMaterial(uint32_t id);
		std::shared_ptr<LoadedTexture> GetTexture(GUID id);

	private:
		uint32_t HashFilepathToID(const std::string& filepath);

		std::unordered_map<GUID, std::shared_ptr<std::vector<Triangle>>> m_Meshes;
		std::unordered_map<GUID, std::shared_ptr<BVH::BVH_data>> m_BVHs;
		std::unordered_map<uint32_t, std::shared_ptr<Material>> m_Materials;
		std::unordered_map<GUID, std::shared_ptr<LoadedTexture>> m_Textures;
	};
}