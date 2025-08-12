#include <imgui.h>
#include "EditorLayer.h"
#include "EditorUI/UtilityUI.h"
#include "EditorUI/ViewportPanel/ViewportPanel.h"
#include "EditorUI/SceneHierarchyPanel/SceneHierarchyPanel.h"
#include "EditorUI/InspectorPanel/InspectorPanel.h"
#include "EditorUI/ProfilerPanel/ProfilerPanel.h"
#include "EditorUI/RenderSettingsPanel/RenderSettingsPanel.h"
#include "EditorUI/ThemePanel/ThemePanel.h"
#include "EditorUI/AssetsPanel/AssetsPanel.h"

namespace Laura
{

	EditorLayer::EditorLayer(std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<ProjectManager> projectManager,
							 std::shared_ptr<ImGuiContext> imGuiContext)
		:	m_EventDispatcher(eventDispatcher),
			m_ProjectManager(projectManager),
			m_Profiler(profiler),

			m_EditorState(std::make_shared<EditorState>()),
			m_ImGuiContext(imGuiContext),
		
			m_Launcher(m_EditorState, m_ProjectManager),

			m_SceneHierarchyPanel(std::make_unique<SceneHierarchyPanel>(m_EditorState, m_ProjectManager)),
			m_InspectorPanel(std::make_unique<InspectorPanel>(m_EditorState, m_ProjectManager)),
			m_ViewportPanel(std::make_unique<ViewportPanel>(m_EditorState, m_ProjectManager)),
			m_ThemePanel(std::make_unique<ThemePanel>(m_EditorState)),
			m_ProfilerPanel(std::make_unique<ProfilerPanel>(m_EditorState, m_Profiler)),
			m_RenderSettingsPanel(std::make_unique<RenderSettingsPanel>(m_EditorState, m_EventDispatcher)),
			m_AssetsPanel(std::make_unique<AssetsPanel>(m_EditorState, m_ProjectManager)){
	}

	void EditorLayer::onAttach() {
		deserializeState(m_EditorState);

		m_InspectorPanel		->init();
		m_SceneHierarchyPanel	->init();
		m_ViewportPanel			->init();
		m_ThemePanel			->init();
		m_ProfilerPanel			->init();
		m_RenderSettingsPanel	->init();
		m_AssetsPanel			->init();
	}

	void EditorLayer::onDetach() {
		serializeState(m_EditorState);
	}

	void EditorLayer::onEvent(std::shared_ptr<IEvent> event) {
		// propagate events to individual panels
		m_InspectorPanel		->onEvent(event);
		m_SceneHierarchyPanel	->onEvent(event);
		m_ViewportPanel			->onEvent(event);
		m_ThemePanel			->onEvent(event);
		m_ProfilerPanel			->onEvent(event);
		m_RenderSettingsPanel	->onEvent(event);
		m_AssetsPanel			->onEvent(event);
	}

	void EditorLayer::onUpdate() {
		m_ImGuiContext->BeginFrame();
		m_EditorState->temp.editorTheme.ApplyAllToImgui(); // apply theme every frame

		if (!m_ProjectManager->ProjectIsOpen()) {
			m_Launcher.OnImGuiRender();
			m_ImGuiContext->EndFrame();
			return;
		}

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		DrawMainMenu();
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);

		m_SceneHierarchyPanel	->OnImGuiRender();
		m_InspectorPanel		->OnImGuiRender();
		m_ThemePanel			->OnImGuiRender();
		m_AssetsPanel			->OnImGuiRender();
		m_RenderSettingsPanel	->OnImGuiRender();
		m_ViewportPanel			->OnImGuiRender();
		m_ProfilerPanel			->OnImGuiRender();

		m_ImGuiContext->EndFrame();
	}


	void EditorLayer::DrawMainMenu() {
		static bool shouldCloseProject = false;

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Project")) {
				if (ImGui::MenuItem("Save")) { m_ProjectManager->SaveProject(); }
				if (ImGui::MenuItem("Close")) { shouldCloseProject = true; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {
				bool themePanelDisabled = m_EditorState->temp.isThemePanelOpen;
				if (themePanelDisabled)			 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_BRUSH " THEMES"  )) { m_EditorState->temp.isThemePanelOpen = true; }
				if (themePanelDisabled)			 { ImGui::EndDisabled(); }
				bool profilerPanelDisabled = m_EditorState->temp.isProfilerPanelOpen;
				if (profilerPanelDisabled)		 { ImGui::BeginDisabled(); }
				if (ImGui::MenuItem(ICON_FA_STOPWATCH " PROFILER")) { m_EditorState->temp.isProfilerPanelOpen = true; }
				if (profilerPanelDisabled)		 { ImGui::EndDisabled(); }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ConfirmWithCancel (
			shouldCloseProject,
			ICON_FA_DIAGRAM_PROJECT " Close Project",
			"Save before closing?",
			ICON_FA_FLOPPY_DISK " Save & Close Project",
			"Close Project",
			"Cancel",
			[&]() { 
				m_ProjectManager->SaveProject();
				m_ProjectManager->CloseProject();
			},
			[&]() { 
				m_ProjectManager->CloseProject(); 
			},
			m_EditorState	
		);
	}
}