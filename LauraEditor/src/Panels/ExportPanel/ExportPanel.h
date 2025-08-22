#pragma once

#include "Laura.h"
#include "EditorState.h"
#include "Panels/IEditorPanel.h"
#include "Export/ExportSettings.h"

namespace Laura
{

	class ExportPanel : public IEditorPanel {
	public:
		ExportPanel(std::shared_ptr<EditorState> editorState, 
			        std::shared_ptr<ProjectManager> projectManager)
			: m_EditorState(editorState)
			, m_ProjectManager(projectManager) 
		{}

		~ExportPanel() = default;

		virtual inline void init() override {}
		virtual void OnImGuiRender() override;
		virtual inline void onEvent(std::shared_ptr<IEvent> event) override {}

		inline void OnPanelOpen() {
			// copy current project name
			strncpy(m_ProjectName, m_ProjectManager->GetProjectName().c_str(), sizeof(m_ProjectName) - 1);
			m_ProjectName[sizeof(m_ProjectName) - 1] = '\0';
			m_Folderpath = "";

			//m_ExportSettings.screenFitMode = m_EditorState->persistent.viewportMode;
		}

	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher;
		std::shared_ptr<ProjectManager> m_ProjectManager;

		bool m_ExportPanelOpen = false;

		// serialized data
		ExportSettings m_ExportSettings;

		// non serialized
		char m_ProjectName[250] = "";
		std::filesystem::path m_Folderpath = "";
	};
}
