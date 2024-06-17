#pragma once
#include <imgui.h>
#include <glm/glm.hpp>

#include <windows.h>
#include <commdlg.h>

/**
* @brief Wrap code in an if statement and set imgui_was_input as true
* @param code - the code to be wrapped
*/
#define IMGUI_INPUT(code) \
    if (code) { \
        was_IMGUI_input = true; \
    }

void genSkyboxGUI(glm::vec3 & groundColor, glm::vec3 & skyColorHorizon, glm::vec3 & skyColorZenith, bool& useCustomSkyboxGradient, bool& was_IMGUI_input, std::string& skyboxFilePath, bool& reloadSkybox , bool disabled)
{
	static std::string skyboxTexName = "None";
	ImGui::Begin("Skybox Settings");
	if (disabled || useCustomSkyboxGradient) { ImGui::BeginDisabled(); }
	if (ImGui::Button("Open...")) {
		OPENFILENAMEA ofn = { sizeof(OPENFILENAMEA) };
		char buff[MAX_PATH] = {};

		ofn.lpstrFilter = "hdr files\0*.hdr\0";

		ofn.lpstrTitle = "Select a skybox";
		ofn.nMaxFile = sizeof(buff);
		ofn.lpstrFile = buff;
		GetOpenFileNameA(&ofn);
		skyboxFilePath = buff;
		skyboxTexName = skyboxFilePath.substr(skyboxFilePath.find_last_of("\\") + 1);
		
		if (skyboxFilePath != "") {
			reloadSkybox = true;
			was_IMGUI_input = true;
		}
	}
	ImGui::Text(("Selected Skybox: " + skyboxTexName).c_str());
	if (disabled || useCustomSkyboxGradient) { ImGui::EndDisabled(); }
	
	if(disabled) { ImGui::BeginDisabled(); }
	IMGUI_INPUT(ImGui::Checkbox("Custom Gradient", &useCustomSkyboxGradient));
	if (useCustomSkyboxGradient) {
		ImGui::Separator();
		IMGUI_INPUT(ImGui::ColorEdit3("GroundColor", &groundColor.x));
		IMGUI_INPUT(ImGui::ColorEdit3("SkyColorHorizon", &skyColorHorizon.x));
		IMGUI_INPUT(ImGui::ColorEdit3("SkyColorZenith", &skyColorZenith.x));
	}

	if (disabled) { ImGui::EndDisabled(); }
	ImGui::End();
	
}

