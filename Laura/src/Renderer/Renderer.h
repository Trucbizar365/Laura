#pragma once

#include "lrpch.h"

#include "Scene/Scene.h"

#include "Assets/Assets.h"

#include "Renderer/IRendererAPI.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/ITexture2D.h"
#include "Renderer/IImage2D.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/IShaderStorageBuffer.h"

namespace Laura 
{

	class Renderer
	{
	private:
		struct Settings
		{
			uint32_t raysPerPixel;
			uint32_t bouncesPerRay;
			bool accumulateFrames;
			bool displayBVH;
			uint32_t maxAABBIntersections;
			glm::vec2 resolution;
		};

		struct Cache
		{
			glm::vec2 resolution = {0.0f, 0.0f};
		};

		struct MeshEntityHandle 
		{
			MeshEntityHandle(uint32_t firstTriIdx = 0, uint32_t triCount = 0, uint32_t firstNodeIdx = 0, uint32_t nodeCount = 0, glm::mat4 transform = {}) 
			:	FirstTriIdx(firstTriIdx),
				TriCount(triCount),
				FirstNodeIdx(firstNodeIdx),
				NodeCount(nodeCount),
				Transform(transform)
			{
			}

			uint32_t FirstTriIdx;
			uint32_t TriCount;
			uint32_t FirstNodeIdx;
			uint32_t NodeCount;
			glm::mat4 Transform;
		};

		struct ParsedScene {
			std::vector<MeshEntityHandle> MeshEntityLookupTable;

			uint32_t SkyboxFirstTexIdx = 0, SkyboxWidth = 0, SkyboxHeight = 0, SkyboxChannels = 0;
			
			bool hasValidCamera = false;
			glm::mat4 CameraTransform;
			float CameraFocalLength = 0;
		};


	public:
		Renderer() = default;
		~Renderer() = default;

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); } // getter
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); } // setter

		void Init();
		std::shared_ptr<IImage2D> Render(const Scene* scene, const Asset::ResourcePool* resourcePool, const Settings& renderSettings);

	private:
		std::shared_ptr<const ParsedScene> Parse(const Scene* scene, const Asset::ResourcePool* resourcePool) const;
		void SetupGPUResource(std::shared_ptr<const ParsedScene> pScene, const Settings& settings);
		std::shared_ptr<IImage2D> Draw();

		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<ITexture2D> m_SkyboxTexture;
		std::shared_ptr<IImage2D> m_Frame;
		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_RenderSettingsUBO, m_EnvironmentUBO, m_ObjectsMetadataUBO;
		std::shared_ptr<IShaderStorageBuffer> m_TransformsSSBO, m_ContinuousMeshesSSBO, m_ContinuousBvhsSSBO, m_MeshMappingsSSBO, m_MeshSizesSSBO, m_BvhMappingsSSBO, m_BVHSizesSSBO;

		Cache m_Cache;
	};
}