#pragma once

#include "core/ILayer.h"
#include "core/IWindow.h"

namespace Laura {

	class ImGuiContextManager
	{
	public:
		ImGuiContextManager(std::shared_ptr<IWindow> window);
		virtual ~ImGuiContextManager();

		void Init();
		void BeginFrame();
		void EndFrame();

	private:
		std::shared_ptr<IWindow> m_Window;
	};

}