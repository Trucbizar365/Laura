#pragma once

#include "core/ILayer.h"
#include "core/IWindow.h"

namespace Laura {

	class ImGuiLayer : public ILayer
	{
	public:
		ImGuiLayer(std::shared_ptr<IWindow> window);
		virtual ~ImGuiLayer();

		void Begin();
		void End();

		void onAttach() override;
		void onDetach() override;
		void onImGuiRender() override;
	private:
		std::shared_ptr<IWindow> m_Window;
	};

}