#include "core/IWindow.h"

#define PLATFORM_WINDOWS


#ifdef PLATFORM_WINDOWS
#include "platform/windows/GLFWWindow.h"
#endif

namespace Laura {

	std::shared_ptr<IWindow> IWindow::createWindow(WindowProps windowProps)
	{
		#ifdef PLATFORM_WINDOWS
			return std::make_shared<GLFWWindowIMPL>(windowProps);
		#else
			std::cout << "Unknown platform!" << std::endl;
			return nullptr;
		#endif
	}
}