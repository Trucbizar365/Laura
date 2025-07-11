#pragma once 
#include "Laura.h"
#include "EditorState.h"
#include "EditorTheme.h"

namespace Laura
{
	class ThemesPanel
	{
	public:
		inline ThemesPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager)
			: m_EditorState(editorState), m_ThemeManager(themeManager) {
		}

		~ThemesPanel() = default;

		void OnImGuiRender();
	private:
		std::shared_ptr<EditorState> m_EditorState;
		std::shared_ptr<ThemeManager> m_ThemeManager;
	};
}