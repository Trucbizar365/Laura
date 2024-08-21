#include "EditorLayer.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Renderer> renderer)
		: m_Renderer(renderer),
		prevViewportWindowSize(glm::ivec2(0, 0)),
		prevViewportWindowPos(glm::ivec2(0, 0)),
		viewportSize(glm::ivec2(0, 0)),
		aspectRatio(16.0f/9.0f)
	{
		setLayerName("EditorLayer");
	}

	void EditorLayer::onAttach()
	{
		// Setting up the Skybox
		m_Scene.skybox.changeType(SkyboxType::SKYBOX_TEXTURE);
		std::string texturePath = EDITOR_RESOURCES_PATH "Skyboxes/kloofendal_48d_partly_cloudy_puresky_4k.hdr";
		m_Scene.skybox.setTexturePath(texturePath);


		// Adding a CAMERA to the scene
		Entity camera = m_Scene.CreateEntity();
		TransformComponent& cameraTransform = camera.AddComponent<TransformComponent>();
		TransformHandler::Translate(cameraTransform, { 0.0f, 40.0f, -200.0f });
		CameraComponent cameraComponent = camera.AddComponent<CameraComponent>();
		// Using the default values for the camera component
		cameraComponent.fov = 30.0f;
		//cameraComponent.aspectRatio = 16.0f/9.0f;


		// Adding a 3D MODEL to the scene
		Entity dragon = m_Scene.CreateEntity();
		MeshComponent& dragonMesh			= dragon.AddComponent<MeshComponent>();
		TransformComponent& dragonTransform	= dragon.AddComponent<TransformComponent>();
		MaterialComponent& dragonMaterial	= dragon.AddComponent<MaterialComponent>();
		//TransformHandler::Translate(dragonTransform, { 0.0f, 0.0f, 0.0f });

		dragonMesh.mesh = MeshLoader::loadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));
		//dragonMesh.mesh = MeshLoader::loadMesh(std::string(EDITOR_RESOURCES_PATH "Models/stanford_bunny_pbr.glb"));
		//dragonMesh.mesh = MeshLoader::loadMesh(std::string(EDITOR_RESOURCES_PATH "Models/sponza_scene.glb"));


		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		#define FRAME_WIDTH 1280.0f
		#define FRAME_HEIGHT 720.0f
		m_Renderer->renderSettings.frameDimentions = glm::vec2(FRAME_WIDTH, FRAME_HEIGHT);
		m_Renderer->renderSettings.raysPerPixel = 1;
		m_Renderer->renderSettings.bouncesPerRay = 5;
		m_Renderer->renderSettings.maxAABBIntersections = 10000;
		m_Renderer->renderSettings.displayBVH = false;

		/// ------------- SCRIPTING -------------- ///

		// Test SCRIPT (will be in its own file in the future)
		class TestScript : public Script
		{
			virtual void OnCreate() override 
			{
				LR_APP_INFO("TestScript::OnCreate (called in scene.OnStart())");
			}

			virtual void OnUpdate() override
			{
				LR_APP_INFO("TestScript::OnUpdate (this should get called every frame)");
			}

			virtual void OnDestroy() override
			{
				LR_APP_INFO("TestScript::OnDestroy (called once the entity gets destroyed)");
			}
		};
		
		// Testing the TEST SCRIPT on a TEST ENTITY
		Entity testEntity = m_Scene.CreateEntity();
		testEntity.AddComponent<ScriptComponent>(new TestScript());
		//m_Scene.DestroyEntity(testEntity); // testing the script's OnDestroy() function

		/// ---------------------------------------- ///

		m_Scene.OnStart();

		// setting up the renderer
		m_Renderer->BeginScene(camera, m_Scene.skybox);
		MeshComponent dragon_mesh = dragon.GetComponent<MeshComponent>();
		TransformComponent dragon_transform = dragon.GetComponent<TransformComponent>();
		MaterialComponent dragon_material = dragon.GetComponent<MaterialComponent>();
		m_Renderer->Submit(dragon_mesh, dragon_transform, dragon_material);
	}

	void EditorLayer::onEvent(Event* event)
	{
	}

	void EditorLayer::onUpdate()
	{
		m_Scene.OnUpdate();
	}


	void EditorLayer::onImGuiRender()
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		bool showDemoWindow = false;
		ImGui::ShowDemoWindow(&showDemoWindow);

		m_Renderer->renderSettings.accumulateFrames = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

		glm::ivec2 viewportDims = glm::vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		if (viewportDims != prevViewportWindowSize)
		{
			prevViewportWindowSize = viewportDims;

			float viewportAspect = float(viewportDims.x) / float(viewportDims.y);
			// check if the parent aspect is smaller or equal to camera aspect (width is the limiting factor so the window will span the entire width)
			if (viewportAspect <= aspectRatio)
			{
				viewportSize.x = viewportDims.x; // width stays the same
				viewportSize.y = ceil(viewportDims.x / aspectRatio); // calculate the height based on the aspect ratio
			}
			// (height is the limiting factor so the window will span the entire height)
			else if (viewportAspect > aspectRatio)
			{
				viewportSize.y = viewportDims.y; // height stays the same
				viewportSize.x = ceil(viewportDims.y * aspectRatio); // calculate the width based on the aspect ratio
			}

			m_Renderer->renderSettings.frameDimentions = glm::ivec2(viewportSize.x, viewportSize.y);
			m_Renderer->renderSettings.accumulateFrames = false;
			m_Renderer->UpdateRenderSettingsUBO();

			topLeftTextureCoords.x = float(viewportDims.x - viewportSize.x) / 2.0f;
			topLeftTextureCoords.y = float(viewportDims.y - viewportSize.y) / 2.0f;
			// viewport offset
			topLeftTextureCoords.x += ImGui::GetWindowPos().x;
			topLeftTextureCoords.y += ImGui::GetWindowPos().y;

			bottomLeftTextureCoords.x = topLeftTextureCoords.x + viewportSize.x;
			bottomLeftTextureCoords.y = topLeftTextureCoords.y + viewportSize.y;
		}


		// check if the viewport window position changed
		glm::ivec2 viewportWindowPos = glm::ivec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		if (viewportWindowPos != prevViewportWindowPos)
		{
			prevViewportWindowPos = viewportWindowPos;

			topLeftTextureCoords.x = (viewportDims.x - viewportSize.x) / 2.0f;
			topLeftTextureCoords.y = (viewportDims.y - viewportSize.y) / 2.0f;
			// viewport offset
			topLeftTextureCoords.x += viewportWindowPos.x;
			topLeftTextureCoords.y += viewportWindowPos.y;

			bottomLeftTextureCoords.x = topLeftTextureCoords.x + viewportSize.x;
			bottomLeftTextureCoords.y = topLeftTextureCoords.y + viewportSize.y;
		}

		std::shared_ptr<IImage2D> RenderedFrame = m_Renderer->RenderScene();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddImage((ImTextureID)RenderedFrame->GetID(), topLeftTextureCoords, bottomLeftTextureCoords, { 0, 1 }, { 1, 0 });

		ImGui::PopStyleVar();
		ImGui::End();
	}

	void EditorLayer::onDetach()
	{
		m_Scene.OnShutdown();
	}

}