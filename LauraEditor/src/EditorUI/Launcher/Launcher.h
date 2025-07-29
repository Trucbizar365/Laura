#pragma once

#include <Laura.h>
#include "EditorState.h"

#include <shobjidl.h>  // IFileDialog
#include <filesystem>
#include <optional>


namespace Laura 
{

	class Launcher {
	public:
		Launcher(std::shared_ptr<EditorState> editorState) 
			: m_EditorState(editorState) {
		}
		~Launcher() = default;
	
		std::optional<std::filesystem::path> ShowFolderPickerDialog();
		void OnImGuiRender();

	private:
		std::shared_ptr<EditorState> m_EditorState;
	};
}
