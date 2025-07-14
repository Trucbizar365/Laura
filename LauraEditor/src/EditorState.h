#pragma once
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <cassert>
#include "Laura.h"
#include "EditorTheme.h"

#define EDITOR_STATE_FILE_PATH EDITOR_RESOURCES_PATH "EditorStatePersistent.yaml"

namespace Laura
{
	enum struct ViewportMode {
		FIT_TO_VIEWPORT,
		STRETCH_TO_VIEWPORT,
		CENTERED
	};

	struct EditorState {
		struct {
			entt::entity selectedEntity = entt::null;
			bool ViewportSettingsPanelOpen = false;
			bool ThemeSettingsPanelOpen = false;
			bool ProfilerPanelOpen = true;
			EditorTheme editorTheme;
		} temp;

		// TO ADD new persistent entries, add them here and update the SERIALIZE and DESERIALIZE functions 
		// (if the type is custom, also create a YAML::convert template specialization)
		struct
		{
			bool doubleConfirmEnabled = false;
			ViewportMode viewportMode = ViewportMode::FIT_TO_VIEWPORT;
			std::filesystem::path editorThemeFilepath = "";
		} persistent;
	};

	bool serializeState(const std::shared_ptr<const EditorState>& state);
	// also deserializes derived state (EditorTheme from editorThemeFilepath)
	bool deserializeState(const std::shared_ptr<EditorState>& state); 
}

template<>
struct YAML::convert<Laura::ViewportMode>
{
	static YAML::Node encode(const Laura::ViewportMode& rhs)
	{
		switch (rhs)
		{
		case Laura::ViewportMode::FIT_TO_VIEWPORT: return Node("FIT_TO_VIEWPORT");
		case Laura::ViewportMode::STRETCH_TO_VIEWPORT: return Node("STRETCH_TO_VIEWPORT");
		case Laura::ViewportMode::CENTERED: return Node("CENTERED");
		}
		return YAML::Node("FIT_TO_VIEWPORT");
	}

	static bool decode(const YAML::Node& node, Laura::ViewportMode& rhs)
	{
		std::string value = node.as<std::string>();
		if (value == "FIT_TO_VIEWPORT") rhs = Laura::ViewportMode::FIT_TO_VIEWPORT;
		else if (value == "STRETCH_TO_VIEWPORT") rhs = Laura::ViewportMode::STRETCH_TO_VIEWPORT;
		else if (value == "CENTERED") rhs = Laura::ViewportMode::CENTERED;
		else rhs = Laura::ViewportMode::FIT_TO_VIEWPORT;
		return true;
	}
};
