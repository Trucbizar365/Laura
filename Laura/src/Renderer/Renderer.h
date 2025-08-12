#pragma once

#include "lrpch.h"
#include "Project/Scene/Scene.h"
#include "Project/Assets/AssetManager.h"
#include "Renderer/RenderSettings.h"
#include "Renderer/IRendererAPI.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/ITexture2D.h"
#include "Renderer/IImage2D.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/IShaderStorageBuffer.h"
#include "Core/Profiler.h"
#include <filesystem>

namespace Laura 
{

	class Renderer {
	private:

		struct Cache {
			glm::uvec2 Resolution{0};
			std::filesystem::path ActiveShaderPath{};
			uint32_t AccumulatedFrames = 0;

			LR_GUID prevSkyboxGuid = LR_GUID::INVALID;
		};

		// Under the std430 - 80 bytes
		struct MeshEntityHandle { 
			MeshEntityHandle(uint32_t firstTriIdx = 0, uint32_t triCount = 0, uint32_t firstNodeIdx = 0, uint32_t nodeCount = 0, glm::mat4 transform = {}) 
			:	FirstTriIdx(firstTriIdx),
				TriCount(triCount),
				FirstNodeIdx(firstNodeIdx),
				NodeCount(nodeCount),
				Transform(transform) {
			}

			glm::mat4 Transform;
			uint32_t FirstTriIdx;
			uint32_t TriCount;
			uint32_t FirstNodeIdx;
			uint32_t NodeCount;
		};

		struct ParsedScene {
			std::vector<MeshEntityHandle> MeshEntityLookupTable; // only renderable entities in the scene

			bool hasValidCamera = false;
			float CameraFocalLength = 0;
			glm::mat4 CameraTransform{};

			LR_GUID skyboxGUID = LR_GUID::INVALID;
		};

	public:
		Renderer(std::shared_ptr<Profiler> profiler)
			: m_Profiler(profiler) {
		};
		~Renderer() = default;

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); } // getter
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); } // setter

		inline void applySettings(RenderSettings renderSettings) { m_RenderSettings = renderSettings; }

		void Init();
		std::shared_ptr<IImage2D> Render(const Scene* scene, const AssetPool* resourcePool);

	private:
		std::shared_ptr<const ParsedScene> Parse(const Scene* scene, const AssetPool* resourcePool) const;
		bool SetupGPUResources(std::shared_ptr<const ParsedScene> pScene, const Scene* scene, const AssetPool* resourcePool);
		void Draw(); // Draws directly to m_Frame


		std::shared_ptr<Profiler> m_Profiler;

		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<IImage2D> m_Frame;
		std::shared_ptr<ITexture2D> m_SkyboxTexture;
		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_SettingsUBO;
		std::shared_ptr<IShaderStorageBuffer> m_MeshEntityLookupSSBO, m_MeshBufferSSBO, m_NodeBufferSSBO, m_IndexBufferSSBO;
		
		Cache m_Cache;
		RenderSettings m_RenderSettings;
		std::filesystem::path m_ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";
	};
}