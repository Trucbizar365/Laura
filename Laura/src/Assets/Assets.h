#pragma once

#include "lrpch.h"

#include "Assets/AssetTypes.h"
#include "Assets/BVHAccel.h"

#include "Core/GUID.h"

#include <filesystem>

namespace Laura::Asset
{
	struct ResourcePool {
		std::unordered_map<LR_GUID, std::shared_ptr<Metadata>> Metadata;
		std::vector<Triangle> MeshBuffer;
		std::vector<uint32_t> IndexBuffer; // indirection between BVHAccel::Node and triangles in ResourcePool::meshBuffer
		std::vector<BVHAccel::Node> NodeBuffer;
		std::vector<unsigned char> TextureBuffer;
	};

	class Manager
	{
	public:
		Manager(ResourcePool* pool = nullptr)
			: resourcePool(pool) 
		{
		}
		~Manager() = default;
		
		inline void SetResourcePool(ResourcePool* pool) {
			resourcePool = pool;
		}

		LR_GUID LoadMesh(const std::filesystem::path& path);
		// 0: original (default), 1: grayscale, 2: grayscale+alpha, 3: RGB, 4: RGBA
		LR_GUID LoadTexture(const std::filesystem::path& path, const int channels = 0);

		template <typename T>
		std::shared_ptr<T> Get(const LR_GUID& guid) {
			if (!ResourcePoolValid(resourcePool))
				return nullptr;
			auto it = resourcePool->Metadata.find(guid);
			if (it == resourcePool->Metadata.end())
				return nullptr;
			return std::dynamic_pointer_cast<T>(it->second);
		}

		ResourcePool* resourcePool = nullptr;
	
	private:
		static inline bool ResourcePoolValid(ResourcePool* resourcePool) {
			if (!resourcePool)
				LR_CORE_WARN("Asset::ResourcePool* in Asset::Manager is nullptr");
		}
	};
}