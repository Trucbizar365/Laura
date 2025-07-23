#include <Laura.h>
#include <LauraEntrypoint.h>
#include "EditorLayer.h"
#include "ImGuiContext.h"  

namespace Laura
{

	class LauraEditor : public Application {
	public:
		LauraEditor()
			: Application() {
		}

		void init() override {
			Application::init();

			_ImGuiContext = std::make_shared<ImGuiContext>(_Window);
			_ImGuiContext->Init();

			_LayerStack->PushLayer(std::make_shared<EditorLayer>(_LayerStack, _ImGuiContext, _ResourcePool, _AssetManager, _Profiler));
		}

		~LauraEditor() {
		}

	protected:
		std::shared_ptr<ImGuiContext> _ImGuiContext;
	};

	Application* CreateApplication() {
		return new LauraEditor();
	}
}