#pragma once
#include "Laura.h"
#include <filesystem> 
#include <yaml-cpp/yaml.h>
#include <deque>
#include <array>
#include <cassert>

namespace Laura
{
	#define EDITOR_THEME_FILE_EXTENSION ".lrtheme"
	
	// how used in code
	enum EditorCol_ {
		EditorCol_Primary1,
		EditorCol_Primary2,
		EditorCol_Primary3,
		EditorCol_Secondary1,
		EditorCol_Secondary2,
		EditorCol_Accent1,
		EditorCol_Accent2,
		EditorCol_Text1,
		EditorCol_Text2,
		EditorCol_Background1,
		EditorCol_Background2,
		EditorCol_Error,
		EditorCol_Warning,
		EditorCol_Success,
		EditorCol_X,
		EditorCol_Y,
		EditorCol_Z,
		EditorCol_COUNT
	};
	
	// how appear serialized in the file
	constexpr const char* EditorColStrings[] = {
		"EditorCol_Primary1",
		"EditorCol_Primary2",
		"EditorCol_Primary3",
		"EditorCol_Secondary1",
		"EditorCol_Secondary2",
		"EditorCol_Accent1",
		"EditorCol_Accent2",
		"EditorCol_Text1",
		"EditorCol_Text2",
		"EditorCol_Background1",
		"EditorCol_Background2",
		"EditorCol_Error",
		"EditorCol_Warning",
		"EditorCol_Success",
		"EditorCol_X",
		"EditorCol_Y",
		"EditorCol_Z"
	};

	static_assert(std::size(EditorColStrings) == EditorCol_COUNT, "Mismatch in enum and string array size");

	class EditorTheme {
	public: 
		EditorTheme() {
			LoadDefaultDark();
			ApplyAllToImgui();
		}

		void LoadDefaultDark(); // main default
		void LoadDefaultLight(); // not used (exported in repo as DefaultLight.lrtheme)

		inline std::pair<bool, std::string> LoadFromFile(const std::filesystem::path& filepath) {
			auto [success, errMsg] = DeserializeFromYAML(filepath);
			if (success) {
				ApplyAllToImgui();
			}
			return { success, errMsg };
		}

		inline std::pair<bool, std::string> SaveToFile(const std::filesystem::path& filepath) {
			return SerializeToYAML(filepath);
		}
			
		inline void PushColor(ImGuiCol_ widget, EditorCol_ col) {
			assert(m_ColorStack.size() < EditorCol_COUNT && "ColorStack.size() exceeds EditorCol_COUNT (did you forget to pop?)");
			// cache old color on the stack
			m_ColorStack.push_front({ widget, colorMap[widget] });
			// set new color	
			ImGui::GetStyle().Colors[widget] = m_ColorPallete[col]; 
			colorMap[widget] = col;
		}

		inline void PopColor(size_t count = 1) {
			assert(0 < count && count <= m_ColorStack.size() && "Too many PopColor() calls (ColorStack already empty)");
			for (size_t i = 0; i < count; i++) {
				// pop the latest widget change and update color 
				auto [widget, col] = m_ColorStack.front();
				m_ColorStack.pop_front();
				// reset to old color
				ImGui::GetStyle().Colors[widget] = m_ColorPallete[col];
				colorMap[widget] = col;
			}
		}

		inline ImVec4& operator[](EditorCol_ col) {
			assert(0 <= col && col < EditorCol_COUNT && "Invalid EditorCol_ passed");
			return m_ColorPallete[col];
		}

		inline const ImVec4& operator[](EditorCol_ col) const {
			assert(0 <= col && col < EditorCol_COUNT && "Invalid EditorCol_ passed");
			return m_ColorPallete[col];
		}

	public:
		std::unordered_map<ImGuiCol_, EditorCol_> colorMap;
		void ApplyAllToImgui();
	private:
		std::deque<std::pair<ImGuiCol_, EditorCol_>> m_ColorStack;
		std::array<ImVec4, EditorCol_COUNT> m_ColorPallete; // stores the actual colors of the theme
		
		std::pair<bool, std::string> SerializeToYAML(const std::filesystem::path& filepath);
		std::pair<bool, std::string> DeserializeFromYAML(const std::filesystem::path& filepath);
	};
};

template <>
struct YAML::convert<ImVec4> {
	static YAML::Node encode(const ImVec4& rhs) {
		YAML::Node node;
		node.SetStyle(YAML::EmitterStyle::Flow);  // Force inline [x, y, z, w] format
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		return node;
	}

	static bool decode(const YAML::Node& node, ImVec4& rhs) {
		if (!node.IsSequence() || node.size() != 4) {
			return false;
		}
		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};

template <>
struct YAML::convert<Laura::EditorTheme> {
	static YAML::Node encode(const Laura::EditorTheme& rhs) {
		YAML::Node node;
		for (size_t i = 0; i < Laura::EditorCol_COUNT; i++) {
			node[Laura::EditorColStrings[i]] = rhs[static_cast<Laura::EditorCol_>(i)];
		}
		return node;
	}

	static bool decode(const YAML::Node& node, Laura::EditorTheme& rhs) {
		if (!node.IsMap()) return false;
		for (size_t i = 0; i < Laura::EditorCol_COUNT; i++) { 
			if (node[Laura::EditorColStrings[i]]) { 
				rhs[static_cast<Laura::EditorCol_>(i)] = node[Laura::EditorColStrings[i]].as<ImVec4>();
			}
		}
		return true;
	}
};
