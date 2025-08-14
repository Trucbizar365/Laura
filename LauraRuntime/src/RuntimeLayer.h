#pragma once

#include <Laura.h>

namespace Laura
{

	class RuntimeLayer : public ILayer {
	public:
		RuntimeLayer(std::shared_ptr<Profiler> profiler,
					std::shared_ptr<IEventDispatcher> eventDispatcher,
					std::shared_ptr<ProjectManager> projectManager
		);

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(std::shared_ptr<IEvent> event) override;

	private:
		// Engine Systems
		std::shared_ptr<Profiler> m_Profiler;
		std::shared_ptr<IEventDispatcher> m_EventDispatcher; // layerstack  
		std::shared_ptr<ProjectManager> m_ProjectManager;
		
		// Simple texture display
		std::shared_ptr<IImage2D> m_CurrentFrame;
		int m_WindowWidth = 1280, m_WindowHeight = 720;
		unsigned int m_Framebuffer;
	};
}