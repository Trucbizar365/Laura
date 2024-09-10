#pragma once 
#include "Laura.h"
#include "EditorState.h"

namespace Laura
{
	class InspectorPanel
	{
	public:
		InspectorPanel() = default;
		~InspectorPanel() = default;

		void OnImGuiRender(std::shared_ptr<Scene> scene, EditorState& editorState);
	};
}