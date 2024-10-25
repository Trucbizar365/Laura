#include "renderer/Renderer.h"
#include "Assets/TextureLoader.h"
#include "Renderer/BVH/BVHBuilder.h"

namespace Laura 
{
	void Renderer::Init()
	{
		m_Frame = IImage2D::Create(nullptr, m_FrameResolution.x, m_FrameResolution.y, 0, Image2DType::LR_READ_WRITE);

		m_CameraUBO = IUniformBuffer::Create(80, 0, BufferUsageType::DYNAMIC_DRAW);
		m_RenderSettingsUBO = IUniformBuffer::Create(32, 2, BufferUsageType::DYNAMIC_DRAW);
		//m_EnvironmentUBO = IUniformBuffer::Create(64, 1, BufferUsageType::DYNAMIC_DRAW);

		m_Shader = IComputeShader::Create(LR_RESOURCES_PATH "Shaders/RayTracingDefault.comp", glm::uvec3(1));
		m_Shader->Bind();
	}

	void Renderer::SetFrameResolution(const glm::vec2& resolution)
	{
		m_FrameResolution = resolution;
		m_Frame = IImage2D::Create(nullptr, m_FrameResolution.x, m_FrameResolution.y, 0, Image2DType::LR_READ_WRITE);
	}

	void Renderer::SubmitScene(std::shared_ptr<RenderableScene> rScene)
	{
		m_SceneValid = rScene->isValid; // this flag determines if the renderer will try to render the scene upon RenderScene() call
		// we still try to update parts which are valid

		UpdateRenderSettingsUBO();
		UpdateCameraUBO(rScene->cameraTransform, rScene->cameraFocalLength);
		
		if (rScene->skyboxDirty)
		{
			m_SkyboxTexture = ITexture2D::Create(rScene->skybox->data, rScene->skybox->width, rScene->skybox->height, 1);
		}

		if (rScene->meshesDirty)
		{
			//for (Entity& model : rScene->models)
			//{
			//	Submit(model);
			//}
			//std::cout << "Submitting model to renderer" << std::endl;
			//// TODO: make the renderer utilize the transform and material components (currently only using the mesh component)
			
			std::shared_ptr<std::vector<Triangle>> mesh = rScene->meshes[0];
			std::shared_ptr<BVH::BVH_data> meshBVH = rScene->BVHs[0];
			
			m_TriangleMeshSSBO = IShaderStorageBuffer::Create(sizeof(Triangle) * meshBVH->TRIANGLES_size, 3, BufferUsageType::STATIC_DRAW);
			m_TriangleMeshSSBO->Bind();
			m_TriangleMeshSSBO->AddData(0, sizeof(Triangle) * meshBVH->TRIANGLES_size, meshBVH->TRIANGLES.data());
			m_TriangleMeshSSBO->Unbind();
			
			m_BVHSSBO = IShaderStorageBuffer::Create(sizeof(BVH::Node) * meshBVH->BVH_size, 4, BufferUsageType::STATIC_DRAW);
			m_BVHSSBO->Bind();
			m_BVHSSBO->AddData(0, sizeof(BVH::Node) * meshBVH->BVH_size, meshBVH->BVH.data());
			m_BVHSSBO->Unbind();
		}
	}

	void Renderer::UpdateCameraUBO(glm::mat4 transform, float focalLength)
	{
		m_CameraUBO->Bind();
		m_CameraUBO->AddData(0, sizeof(glm::mat4), &transform);
		m_CameraUBO->AddData(64, sizeof(float), &focalLength);
		m_CameraUBO->Unbind();
	}

	void Renderer::UpdateSkyboxUBO(std::shared_ptr<LoadedTexture> skyboxTex)
	{
		

		//m_EnvironmentUBO->Bind();
		//m_EnvironmentUBO->AddData(0, sizeof(glm::vec3), &skybox.getGroundColor());
		//m_EnvironmentUBO->AddData(16, sizeof(glm::vec3), &skybox.getHorizonColor());
		//m_EnvironmentUBO->AddData(32, sizeof(glm::vec3), &skybox.getZenithColor());
		//bool useGradient = (skybox.getType() == SkyboxType::SKYBOX_GRADIENT) ? true : false;
		//m_EnvironmentUBO->AddData(48, sizeof(bool), &useGradient);
		//m_EnvironmentUBO->Unbind();
	}

	void Renderer::UpdateRenderSettingsUBO()
	{
		m_RenderSettingsUBO->Bind();
		m_RenderSettingsUBO->AddData(0, sizeof(uint32_t), &renderSettings.raysPerPixel);
		m_RenderSettingsUBO->AddData(4, sizeof(uint32_t), &renderSettings.bouncesPerRay);
		m_RenderSettingsUBO->AddData(8, sizeof(uint32_t), &renderSettings.maxAABBIntersections);
		m_AccumulateFrameCount = (!renderSettings.accumulateFrames) ? 0 : m_AccumulateFrameCount++; //TODO: this should not be in the renderer
		m_RenderSettingsUBO->AddData(12, sizeof(uint32_t), &m_AccumulateFrameCount);
		m_RenderSettingsUBO->AddData(16, sizeof(bool), &renderSettings.displayBVH);
		m_RenderSettingsUBO->Unbind();
	}

	void Renderer::Submit(const Entity& model)
	{
		
	}

	std::shared_ptr<IImage2D> Renderer::RenderScene()
	{
		if (!m_SceneValid) { return nullptr; }

		m_Shader->Bind();
		m_Shader->setWorkGroupSizes(glm::uvec3(ceil(m_FrameResolution.x / 8),
			                                   ceil(m_FrameResolution.y / 4),
			                                   1));
		m_Shader->Dispatch();

		return m_Frame;
	}
}