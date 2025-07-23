#include "EditorLayer.h"
#include "imgui.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::weak_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<Renderer> renderer,
							 std::shared_ptr<Asset::ResourcePool> resourcePool,
							 std::shared_ptr<Asset::Manager> assetManager,
							 std::shared_ptr<Profiler> profiler)
		:	m_EventDispatcher(eventDispatcher),
			m_Renderer(renderer),
			m_ResourcePool(resourcePool),
			m_AssetManager(assetManager),
			m_Profiler(profiler),

			m_EditorState(std::make_shared<EditorState>()),
			m_InspectorPanel(m_EditorState),
			m_SceneHierarchyPanel(m_EditorState),
			m_ThemePanel(m_EditorState),
			m_ProfilerPanel(m_EditorState),
			m_RenderSettingsPanel(m_EditorState),
			m_AssetsPanel(m_EditorState, m_AssetManager, m_ResourcePool){
	}


	void EditorLayer::onAttach() {
		deserializeState(m_EditorState);
		m_AssetManager->SetResourcePool(m_ResourcePool.get());

		// Most of these are default arguments (not necessary to specify but showing them for clarity)
		m_Renderer->settings.skyboxGuid = m_AssetManager->LoadAsset(EDITOR_RESOURCES_PATH "Skyboxes/kloofendal_48d_partly_cloudy_puresky_4k.hdr");
		m_Renderer->settings.raysPerPixel = 1;
		m_Renderer->settings.bouncesPerRay = 5;
		m_Renderer->settings.maxAABBIntersections = 500;
		m_Renderer->settings.displayBVH = false;
		m_Renderer->settings.ShouldAccumulate = false;

		// The FRAME_WIDTH and FRAME_HEIGHT define the dimensions of the render frame.
		// These values represent the actual number of pixels that the renderer will process to produce the final image.
		// Note: These dimensions are different from the size or aspect ratio of the ImGui viewport window in the editor.
		// The camera's aspect ratio only stretches the image to fit the viewport window correctly
		m_Renderer->settings.Resolution = glm::uvec2(1200, 800);
		m_Renderer->settings.ComputeShaderPath = LR_RESOURCES_PATH "Shaders/PathTracing.comp";
		m_Renderer->Init();
		if (auto scene = m_Scene.lock()) { scene->OnStart(); }
	}

	void EditorLayer::onEvent(std::shared_ptr<IEvent> event) {
		std::cout << event->GetType() << std::endl;
		if (event->GetType() == EventType::SCENE_LOADED_EVENT) {
			m_Scene = std::dynamic_pointer_cast<SceneLoadedEvent>(event)->scene;
		}
	}


	void EditorLayer::onUpdate() {
	}

	void EditorLayer::DrawMainMenu() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::BeginMenu("Scene")) {
					if (ImGui::MenuItem("New")) {
						m_EventDispatcher->dispatchEvent(std::make_shared<SceneCreateEvent>());
					}
					if (ImGui::MenuItem("Close")) {
						m_EventDispatcher->dispatchEvent(std::make_shared<SceneCloseEvent>());
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {
				bool themePanelDisabled = m_EditorState->temp.isThemePanelOpen;
				if (themePanelDisabled)			 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem("Themes"  )) { m_EditorState->temp.isThemePanelOpen = true; }
				if (themePanelDisabled)			 { ImGui::EndDisabled(); }
				bool profilerPanelDisabled = m_EditorState->temp.isProfilerPanelOpen;
				if (profilerPanelDisabled)		 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem("Profiler")) { m_EditorState->temp.isProfilerPanelOpen = true; }
				if (profilerPanelDisabled)		 { ImGui::EndDisabled(); }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	// main rendering function called every frame
	void EditorLayer::onImGuiRender() {
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		DrawMainMenu();

		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);

		m_SceneHierarchyPanel.OnImGuiRender(m_Scene);
		m_InspectorPanel.OnImGuiRender(m_Scene);
		m_ThemePanel.OnImGuiRender();
		m_AssetsPanel.OnImGuiRender();
		m_RenderSettingsPanel.OnImGuiRender();

		// RENDERER RENDERING // 
		std::shared_ptr<IImage2D> RenderedFrame = nullptr;
		if (auto scene = m_Scene.lock()) {
			RenderedFrame = m_Renderer->Render(scene.get(), m_ResourcePool.get());
		}
		m_ViewportPanel.OnImGuiRender(RenderedFrame, m_EditorState);
		m_ProfilerPanel.OnImGuiRender(m_Profiler);
	}

	void EditorLayer::onDetach() {
		serializeState(m_EditorState);
	}
}