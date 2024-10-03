#pragma once
#include "Laura.h"
#include "EditorTheme.h"

namespace Laura
{
	// How to use: call ImGui::OpenPopup(); with the same popupTitle as the first argument before calling this function
	// Returns true if the user clicked the "Yes" button, false otherwise
	bool OpenConfirmationPopup(const char* popupTitle, const char* popupMessage, std::shared_ptr<ThemeManager> themeManager);
}