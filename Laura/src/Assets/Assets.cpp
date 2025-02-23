#include "Assets.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image/stb_image.h>

namespace Laura::Asset
{

	LR_GUID Manager::LoadMesh(const std::filesystem::path& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string(), aiProcessPreset_TargetRealtime_MaxQuality);
        if (!scene) {
            LR_CORE_CRITICAL("Failed to load mesh: {}", path.string());
            return LR_GUID(0);
        }

        size_t numTris = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            numTris += scene->mMeshes[i]->mNumFaces;
        }

        std::vector<Triangle>& meshBuffer = resourcePool->MeshBuffer;

        auto metadata = std::make_shared<MeshMetadata>();
        metadata->firstTriIdx = meshBuffer.size();
        metadata->TriCount = numTris;
        metadata->path = path;
        
        meshBuffer.reserve(meshBuffer.size() + metadata->TriCount);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            const aiMesh* subMesh = scene->mMeshes[i];
            const auto verts = subMesh->mVertices;
            for (unsigned int j = 0; j < subMesh->mNumFaces; ++j) {
                const auto& face = subMesh->mFaces[j];

                auto idxs = face.mIndices;
                meshBuffer.emplace_back( Triangle({ 
                    glm::vec3(verts[idxs[0]].x, verts[idxs[0]].y, verts[idxs[0]].z),
                    glm::vec3(verts[idxs[1]].x, verts[idxs[1]].y, verts[idxs[1]].z),
                    glm::vec3(verts[idxs[2]].x, verts[idxs[2]].y, verts[idxs[2]].z)
                    })
                );
            }
        }

        LR_CORE_INFO("Loaded {} triangles from mesh: {}", numTris, path.string());

        BVHAccel BVH(meshBuffer, metadata->firstTriIdx, metadata->TriCount); // pass in the mesh
        BVH.Build(resourcePool->NodeBuffer, resourcePool->IndexBuffer, metadata->firstNodeIdx, metadata->nodeCount); // populate in place

        LR_GUID guid;
        resourcePool->Metadata[guid] = metadata;
        return guid;
    }

	LR_GUID Manager::LoadTexture(const std::filesystem::path& path, const int desiredChannels)
	{
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1); // ensure (0,0) is bottom-left
		unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, desiredChannels);
        if (!data) {
			LR_CORE_CRITICAL("AssetLoader::LoadTexture() failed to load: {0}", path.string());
			return LR_GUID(0);
		}
        
        std::vector<unsigned char>& textureBuffer = resourcePool->TextureBuffer;

        auto metadata = std::make_shared<TextureMetadata>();
        metadata->texStartIdx = textureBuffer.size();
        metadata->width = width;
        metadata->height = height;
        metadata->channels = (desiredChannels == 0) ? channels : desiredChannels;
        metadata->path = path;

        const size_t totalBytes = metadata->width * metadata->height * metadata->channels;
        textureBuffer.reserve(textureBuffer.size() + totalBytes);
        textureBuffer.insert(textureBuffer.end(), data, data + totalBytes);
        stbi_image_free(data);
		
        LR_GUID guid;
        resourcePool->Metadata[guid] = metadata;
		return guid;
	}
}