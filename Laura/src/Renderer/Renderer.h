#pragma once

#include "lrpch.h"

#include "Scene/Entity.h"
#include "Scene/Skybox.h"
#include "Scene/Components.h"

#include "Renderer/IRendererAPI.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/ITexture2D.h"
#include "Renderer/IImage2D.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/IShaderStorageBuffer.h"

#include "Assets/TextureLoader.h" // LoadedTexture struct
#include "Scene/SceneManager.h"
#include "Assets/AssetManager.h"

namespace Laura 
{

	class Renderer
	{
	private:
		struct RenderSettings
		{
			uint32_t raysPerPixel;
			uint32_t bouncesPerRay;
			bool accumulateFrames;
			bool displayBVH;
			uint32_t maxAABBIntersections;
		};
	public:
		inline Renderer(std::shared_ptr<AssetManager> assetManager)
			: m_AssetManager(assetManager), m_AccumulateFrameCount(0), m_CachedSkyboxID(0), m_CachedMeshCount(0), m_FrameResolution(300, 100)
		{
		}
		~Renderer() = default;

		RenderSettings renderSettings;

		inline static IRendererAPI::API GetAPI() { return IRendererAPI::GetAPI(); }
		inline static void SetAPI(IRendererAPI::API api) { IRendererAPI::SetAPI(api); }

		// Initializes buffers (camera, render settings - things which we know will be used every frame)
		void Init();
		void SubmitScene(std::shared_ptr<RenderableScene> rScene);
		std::shared_ptr<IImage2D> RenderScene();

		void SetFrameResolution(const glm::vec2& resolution);

	private:
		void UpdateCameraUBO(Entity camera);
		void UpdateSkyboxUBO(GUID skyboxID);
		void UpdateRenderSettingsUBO();
		void Submit(const Entity& model);

		glm::vec2 m_FrameResolution;

		std::shared_ptr<IComputeShader> m_Shader;
		std::shared_ptr<ITexture2D> m_SkyboxTexture;
		std::shared_ptr<IImage2D> m_Frame;
		std::shared_ptr<IUniformBuffer> m_CameraUBO, m_RenderSettingsUBO, m_EnvironmentUBO;
		std::shared_ptr<IShaderStorageBuffer> m_TriangleMeshSSBO, m_BVHSSBO;

		uint32_t m_AccumulateFrameCount;
		uint32_t m_CachedMeshCount;
		GUID m_CachedSkyboxID;

		bool m_SceneReadyForRendering = false; // currently the only requirement for rendering is having a camera with a transform in the scene

		std::shared_ptr<AssetManager> m_AssetManager;
	};

}