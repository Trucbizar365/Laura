#include <Laura.h>
#include <LauraEntrypoint.h>
#include "RuntimeLayer.h"

namespace Laura
{

	class LauraRuntime : public Application {
	public:
		LauraRuntime()
			: Application() {
		}

		void init() override {
			Application::init();

			_LayerStack->PushLayer(std::make_shared<RuntimeLayer>(_Profiler, _LayerStack, _ProjectManager));
		}

		~LauraRuntime() {

		}

	};

	Application* CreateApplication() {
		return new LauraRuntime();
	}
}
