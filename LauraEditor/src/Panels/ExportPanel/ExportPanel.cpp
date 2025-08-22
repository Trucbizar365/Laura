#include "ExportPanel.h"
#include <IconsFontAwesome6.h>
#include "Platform/Windows/Dialogs/FolderPickerDialog.h"
#include "Export/ExportSettings.h"
#include "Export/ExportUtilities.h"

namespace Laura
{

	void Laura::ExportPanel::OnImGuiRender() {
		EditorTheme& theme = m_EditorState->temp.editorTheme;

		if (m_EditorState->temp.shouldOpenExportPanel && !m_ExportPanelOpen) {
			m_EditorState->temp.shouldOpenExportPanel = false; // reset flag
			m_ExportPanelOpen = true;
			OnPanelOpen();
		}

        if (!m_ExportPanelOpen) {
            return;
        }

        auto drawLabel = [&](const char* label) {
            theme.PushColor(ImGuiCol_Text, EditorCol_Text2);
            ImGui::Text("%s", label);
            theme.PopColor();
            ImGui::SameLine(150.0f);
        };

		float margin = 3.0f;
        theme.PushColor(ImGuiCol_WindowBg, EditorCol_Background3);
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 250), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::Begin(ICON_FA_ARROW_UP_FROM_BRACKET " Export", &m_ExportPanelOpen, ImGuiWindowFlags_NoDocking); 
        if (m_EditorState->temp.isInRuntimeSimulation) {
            ImGui::BeginDisabled();
        }


		// project name input
		drawLabel("Executable Name: ");
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - margin);
		ImGui::InputTextWithHint("##projectName-input", "Project name", m_ProjectName, sizeof(m_ProjectName));


		// folder selection dialog
		drawLabel("Location");
		std::string buttonLabel = m_Folderpath.empty() ? "Select folder..." : m_Folderpath.string();
		std::string buttonId = "##folder-button";
		EditorCol_ textCol = m_Folderpath.empty() ? EditorCol_Text2 : EditorCol_Text1;
		theme.PushColor(ImGuiCol_Button, EditorCol_Primary3);
		theme.PushColor(ImGuiCol_Text, textCol);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2{ 0.0f, 0.5f });
		if (ImGui::Button((buttonLabel + buttonId).c_str(), { ImGui::GetContentRegionAvail().x - margin, 0 })) {
			if (auto path = FolderPickerDialog("Select folder"); !path.empty()) {
				m_Folderpath = path;
			}
		}
		ImGui::PopStyleVar();
		theme.PopColor(2);


		drawLabel("Fullscreen");
		ImGui::Checkbox("##RuntimeFullscreen", &m_ExportSettings.inFullscreen);


		drawLabel("Viewport Scaling:");
		int modeInt = static_cast<int>(m_ExportSettings.screenFitMode);
		theme.PushColor(ImGuiCol_FrameBg, EditorCol_Background1);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - margin);
		if (ImGui::Combo("##Viewport Mode", &modeInt, ScreenFitModeStr, IM_ARRAYSIZE(ScreenFitModeStr))) {
			m_ExportSettings.screenFitMode = static_cast<ScreenFitMode>(modeInt);
		}
		theme.PopColor();	
		ImGui::Dummy({ 0.0f, 20.0f });


		const char* label = ICON_FA_ARROW_UP_FROM_BRACKET " Export";
		float totalWidth = ImGui::GetContentRegionAvail().x;
		float buttonWidth = ImGui::GetStyle().FramePadding.x * 2 + ImGui::CalcTextSize(label).x;
		ImGui::SetCursorPosX((totalWidth - buttonWidth) / 2.0f);
		if (ImGui::Button(label, {buttonWidth, 30.0f})) {
			ExportProject(m_ProjectName, m_Folderpath, m_ProjectManager->GetProjectFolder(), m_ExportSettings);
		}

        if (m_EditorState->temp.isInRuntimeSimulation) {
            ImGui::EndDisabled();
        }
        ImGui::End();
        theme.PopColor(); // windowBg
	}
}
