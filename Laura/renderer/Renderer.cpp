//#include "renderer/Renderer.h"
//
//namespace Laura 
//{
//	Renderer::Renderer()
//		: m_AccumulateFrameCount(0)
//	{
//	}
//
//	void Renderer::BeginScene(const Camera& camera, const EnvironmentEntity& environment, const RenderSettings& renderSettings)
//	{
//		m_CameraUBO = IUniformBuffer::Create(72, 0, BufferUsageType::DYNAMIC_DRAW);
//		m_SkyboxTexture = ITexture::Create(environment.skybox.getTexturePath(), 1);
//		m_EnvironmentUBO = IUniformBuffer::Create(64, 1, BufferUsageType::DYNAMIC_DRAW);
//		m_RenderSettingsUBO = IUniformBuffer::Create(20, 2, BufferUsageType::DYNAMIC_DRAW);
//		
//		m_Texture = ITexture::Create(renderSettings.viewportDimensions.x, renderSettings.viewportDimensions.y, 4, 0);
//		m_Shader = IComputeShader::Create(CORE_RESOURCES_PATH "shaders/ComputeRayTracing.comp", glm::uvec3(1));
//		// could potentially split the BVH rendering mode to another shader altogether
//		m_Shader->Bind();
//
//		// Pixel Picking (we will see)
//		//m_rtx_parameters_UBO->AddData(80, sizeof(glm::vec3), &rtx_uniform_parameters.pixelGlobalInvocationID);
//
//		UpdateCamera(camera);
//		UpdateEnvironment(environment);
//		UpdateRenderSettings(renderSettings);
//	}
//
//	void Renderer::UpdateCamera(const Camera& camera)
//	{
//		m_CameraUBO->Bind();
//		m_CameraUBO->AddData(0, sizeof(glm::mat4), &camera.transform.toMatrix());
//		m_CameraUBO->AddData(64, sizeof(float), &camera.focalLength);
//		m_CameraUBO->AddData(68, sizeof(float), &camera.aspectRatio);
//		m_CameraUBO->Unbind();
//	}
//
//	void Renderer::UpdateEnvironment(const EnvironmentEntity& environment)
//	{
//		m_EnvironmentUBO->Bind();
//		m_EnvironmentUBO->AddData(0, sizeof(glm::vec3), &environment.skybox.getGroundColor());
//		m_EnvironmentUBO->AddData(16, sizeof(glm::vec3), &environment.skybox.getHorizonColor());
//		m_EnvironmentUBO->AddData(32, sizeof(glm::vec3), &environment.skybox.getZenithColor());
//		bool useGradient = (environment.skybox.getType() == SkyboxType::SKYBOX_GRADIENT) ? true : false;
//		m_EnvironmentUBO->AddData(48, sizeof(bool), &useGradient);
//		m_EnvironmentUBO->Unbind();
//	}
//
//	void Renderer::UpdateRenderSettings(const RenderSettings& renderSettings)
//	{
//		m_RenderSettingsUBO->Bind();
//		m_RenderSettingsUBO->AddData(0, sizeof(uint32_t), &renderSettings.raysPerPixel);
//		m_RenderSettingsUBO->AddData(4, sizeof(uint32_t), &renderSettings.bouncesPerRay);
//		m_RenderSettingsUBO->AddData(8, sizeof(uint32_t), &renderSettings.maxAABBIntersections);
//		m_AccumulateFrameCount = (!renderSettings.accumulateFrames) ? 0 : m_AccumulateFrameCount++;
//		m_RenderSettingsUBO->AddData(12, sizeof(uint32_t), &m_AccumulateFrameCount);
//		m_RenderSettingsUBO->AddData(16, sizeof(bool), &renderSettings.displayBVH);
//		m_RenderSettingsUBO->Unbind();
//	}
//
//	void Renderer::SubmitMesh(const MeshComponent& mesh, uint32_t meshID)
//	{
//	}
//
//	void Renderer::RemoveMesh(uint32_t meshID)
//	{
//	}
//	
//	void Renderer::SubmitSphere(const SphereComponent& sphere, uint32_t sphereID)
//	{
//	}
//
//	void Renderer::RemoveSphere(uint32_t sphereID)
//	{
//	}
//
//	void Renderer::SubmitPlane(const PlaneComponent& plane, uint32_t planeID)
//	{
//	}
//
//	void Renderer::RemovePlane(uint32_t planeID)
//	{
//	}
//
//	ITexture Renderer::RenderScene()
//	{
//		return ITexture();
//	}
//
//	void Renderer::EndScene()
//	{
//	}
//}