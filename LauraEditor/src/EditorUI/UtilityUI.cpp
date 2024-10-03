#include "UtilityUI.h"

namespace Laura
{
	bool OpenConfirmationPopup(const char* popupTitle, const char* popupMessage, std::shared_ptr<ThemeManager> themeManager)
	{
		bool hasConfirmed = false;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		std::cout << "Iam here\n";
		if (ImGui::BeginPopupModal(popupTitle, NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(popupMessage);

			float panelWidth = ImGui::GetContentRegionAvail().x;
			float buttonWidth = panelWidth * 0.5f - 5.0f;

			ImGui::Dummy({ 5.0f, 0.0f });
			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->ButtonGray);
			if (ImGui::Button("Yes", ImVec2(buttonWidth, 0)))
			{
				hasConfirmed = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(buttonWidth, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			themeManager->ImGuiSet(ImGuiCol_Button, themeManager->GetActiveTheme()->DefaultButton);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
		return hasConfirmed;
	}
}