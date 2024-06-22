#pragma once
#include <imgui.h>
#include <glm/glm.hpp>
#include <string>

/**
* @brief Wrap code in an if statement and set imgui_was_input as true
* @param code - the code to be wrapped
*/
#define IMGUI_INPUT(code) \
    if (code) { \
        was_IMGUI_input = true; \
    }

void genSkyboxGUI(glm::vec3 & groundColor, glm::vec3 & skyColorHorizon, glm::vec3 & skyColorZenith, bool& useCustomSkyboxGradient, bool& was_IMGUI_input, std::string & skyboxFilePath, bool& reloadSkybox, bool disabled);

