#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "AssetManager.h"
#include "Project/Assets/AssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image/stb_image.h>

namespace Laura
{
    LR_GUID AssetManager::LoadMesh(const std::filesystem::path& assetpath) {
		auto timerStart = std::chrono::high_resolution_clock::now();
       

        if (!m_AssetPool) {
            LOG_ENGINE_CRITICAL("Asset::Manager::AddIntoAssetPool({0}) called before a valid AssetPool has been assigned!", assetpath.string());
            return LR_GUID::INVALID;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(assetpath.string(), aiProcessPreset_TargetRealtime_MaxQuality);
        if (!scene) {
            LOG_ENGINE_CRITICAL("Asset::Manager::AddIntoAssetPool({0}) failed to load!", assetpath.string());
            return LR_GUID::INVALID;
        }

        // count the number of triangles in file
        size_t numTris = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            numTris += scene->mMeshes[i]->mNumFaces;
        }
        
        std::vector<Triangle>& meshBuffer = m_AssetPool->MeshBuffer;
        // populate some metadata
        auto metadata = std::make_shared<MeshMetadata>();
        metadata->firstTriIdx = meshBuffer.size();
        metadata->TriCount = numTris;
        auto metadataExtension = std::make_shared<MeshMetadataExtension>();
        metadataExtension->sourcePath = assetpath;
        metadataExtension->fileSizeInBytes = std::filesystem::file_size(assetpath);
        
        meshBuffer.reserve(meshBuffer.size() + metadata->TriCount);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            const aiMesh* subMesh = scene->mMeshes[i];
            const auto verts = subMesh->mVertices;
            for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
                const auto& face = subMesh->mFaces[j];

                if (face.mNumIndices != 3) {
                    continue;
                }

                auto idxs = face.mIndices;
                meshBuffer.emplace_back( Triangle({ 
                    glm::vec4(verts[idxs[0]].x, verts[idxs[0]].y, verts[idxs[0]].z, 0.0f),
                    glm::vec4(verts[idxs[1]].x, verts[idxs[1]].y, verts[idxs[1]].z, 0.0f),
                    glm::vec4(verts[idxs[2]].x, verts[idxs[2]].y, verts[idxs[2]].z, 0.0f)
                    })
                );
            }
        }
        m_AssetPool->MarkUpdated(AssetPool::ResourceType::MeshBuffer);

        LOG_ENGINE_INFO("Asset::Manager::AddIntoAssetPool({0}) loaded {1} triangles.", assetpath.string(), numTris);

        BVHAccel BVH(meshBuffer, metadata->firstTriIdx, metadata->TriCount); // pass in the mesh
        BVH.Build(m_AssetPool->NodeBuffer, m_AssetPool->IndexBuffer, metadata->firstNodeIdx, metadata->nodeCount); // populate in place
        m_AssetPool->MarkUpdated(AssetPool::ResourceType::NodeBuffer);
        m_AssetPool->MarkUpdated(AssetPool::ResourceType::IndexBuffer);

        LR_GUID guid;

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		metadataExtension->loadTimeMs = std::chrono::duration<double, std::milli>(end - timerStart).count();
        
        m_AssetPool->Metadata[guid] = { metadata, metadataExtension };
        m_AssetPool->MarkUpdated(AssetPool::ResourceType::Metadata);
        return guid;
    }


	LR_GUID AssetManager::LoadTexture(const std::filesystem::path& assetpath, const int desiredChannels) {
		auto timerStart = std::chrono::high_resolution_clock::now();
        if (!m_AssetPool){
            LOG_ENGINE_CRITICAL("Asset::Manager::LoadTexture({0}, {1}) called before a valid AssetPool has been assigned!", assetpath.string(), desiredChannels);
            return LR_GUID::INVALID;
        }

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1); // ensure (0,0) is bottom-left
		unsigned char* data = stbi_load(assetpath.string().c_str(), &width, &height, &channels, desiredChannels);
        if (!data) {
			LOG_ENGINE_CRITICAL("Asset::Manager::LoadTexture({0}, {1}) failed to load!", assetpath.string(), desiredChannels);
			return LR_GUID::INVALID;
		}
        
        std::vector<unsigned char>& textureBuffer = m_AssetPool->TextureBuffer;

        auto metadata = std::make_shared<TextureMetadata>();
        metadata->texStartIdx = textureBuffer.size();
        metadata->width = width;
        metadata->height = height;
        metadata->channels = (desiredChannels == 0) ? channels : desiredChannels;

        auto metadataExtension = std::make_shared<TextureMetadataExtension>();
        metadataExtension->sourcePath = assetpath;
        metadataExtension->fileSizeInBytes = std::filesystem::file_size(assetpath);

        const size_t totalBytes = metadata->width * metadata->height * metadata->channels;
        textureBuffer.reserve(textureBuffer.size() + totalBytes);
        textureBuffer.insert(textureBuffer.end(), data, data + totalBytes);
        m_AssetPool->MarkUpdated(AssetPool::ResourceType::TextureBuffer);
        stbi_image_free(data);
	    	
        LR_GUID guid;

		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		metadataExtension->loadTimeMs = std::chrono::duration<double, std::milli>(end - timerStart).count();
        
        m_AssetPool->Metadata[guid] = { metadata, metadataExtension };
        m_AssetPool->MarkUpdated(AssetPool::ResourceType::Metadata);
		return guid;
	}


    LR_GUID AssetManager::AddIntoAssetPool(const std::filesystem::path& assetpath) {
        if (!assetpath.has_extension()) {
            LOG_ENGINE_ERROR("AddIntoAssetPool: File has no extension: {}", assetpath.string());
            return LR_GUID::INVALID;
        }

        std::string ext = assetpath.extension().string();
        for ( const auto& SUPPORTED_EXT : SUPPORTED_MESH_FILE_FORMATS) {
            if (ext == SUPPORTED_EXT) {
                return AddIntoAssetPool(assetpath);
            }
        }
        for (auto SUPPORTED_EXT : SUPPORTED_TEXTURE_FILE_FORMATS) {
            if (ext == SUPPORTED_EXT) {
                return LoadTexture(assetpath, 4);
            }
        }

        LOG_ENGINE_WARN("AddIntoAssetPool: Unknown file extension: {}", ext);
        return LR_GUID::INVALID;
    }

    bool AssetManager::SerializeAssetPool(const std::filesystem::path& projectFolderpath) {
        // TODO
        // any .lrmeta which is not within the assetpool metadata but has .lrmeta in the folder should be removed (cleanup of deleted assets)
        // create a .lrmeta file for each asset in the assetpool only if the asset file exists in the directory - take the metadata filepath
        // done
        return true;
    }

    bool AssetManager::DeserializeAssetPool(const std::filesystem::path& projectFolderpath) {
        // TODO
        // read all .lrmeta files in the projectFolderpath
        // load all assets with filepath same as .lrmeta but stripped of the .lrmeta extension only if also has a pair of the actual asset file next to it
        // done
        return true;
    }
}