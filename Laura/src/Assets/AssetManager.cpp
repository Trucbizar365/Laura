#include "AssetManager.h"

#include "Assets/MeshLoader.h"
#include "Assets/TextureLoader.h"

namespace Laura
{
	GUID AssetManager::LoadMesh(const std::string& filepath)
	{
		auto mesh = MeshLoader::loadMesh(filepath);
		if (mesh)
		{
			GUID id;
			m_Meshes[id] = mesh;
			BVH::BVH_data meshBVH = BVH::construct(mesh, BVH::Heuristic::SURFACE_AREA_HEURISTIC_BUCKETS);
			m_BVHs[id] = std::make_shared<BVH::BVH_data>(meshBVH);
			return id;
		}

		return GUID(0); // invalid id
	}

	GUID AssetManager::LoadTexture(const std::string& filepath, uint32_t channels)
	{
		GUID id;
		LoadedTexture tex = TextureLoader::loadTexture(filepath, channels);

		m_Textures[id] = std::make_shared<LoadedTexture>(tex);
		return id;
	}

	uint32_t AssetManager::LoadMaterial(const std::string& filepath)
	{
		return 0;
	}

	void AssetManager::UnloadTexture(GUID id)
	{
		m_Textures.erase(id);
	}

	void AssetManager::UnloadMesh(GUID id)
	{
		m_Meshes.erase(id);
		m_BVHs.erase(id);
	}

	void AssetManager::UnloadMaterial(uint32_t id)
	{
		m_Materials.erase(id);
	}

	std::shared_ptr<std::vector<Triangle>> AssetManager::GetMesh(GUID id)
	{
		return m_Meshes.at(id);
	}

	std::shared_ptr<BVH::BVH_data> AssetManager::GetBVH(GUID id)
	{
		return m_BVHs.at(id);
	}

	std::shared_ptr<Material> AssetManager::GetMaterial(uint32_t id)
	{
		return m_Materials.at(id);
	}

	std::shared_ptr<LoadedTexture> AssetManager::GetTexture(GUID id)
	{
		return m_Textures.at(id);
	}


	uint32_t AssetManager::HashFilepathToID(const std::string& filepath)
	{
		return std::hash<std::string>{}(filepath) + 1; // +1 to reserve 0 for invalid id
	}
}