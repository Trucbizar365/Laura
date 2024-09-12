#pragma once
#include <Laura.h>

namespace Laura
{
	enum struct ViewportMode
	{
		FIT_TO_VIEWPORT,
		STRETCH_TO_VIEWPORT,
		CENTERED
	};

	// Default values for the editor state
	struct EditorState
	{
		entt::entity selectedEntity = entt::null;
		/// VIEWPORT ///
		bool ViewportSettingsPanelOpen = false;
		ViewportMode viewportMode = ViewportMode::FIT_TO_VIEWPORT;
	};
}