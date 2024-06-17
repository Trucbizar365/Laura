#include <imgui.h>

void genSceneHierarchy(bool disabled)
{
	if (disabled) { ImGui::BeginDisabled(); }
	ImGui::Begin("Scene Hierarchy");
	ImGui::End();
	if (disabled) { ImGui::EndDisabled(); }
}