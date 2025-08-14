#include "RuntimeLayer.h"
#include <GL/glew.h>

namespace Laura
{
	RuntimeLayer::RuntimeLayer(std::shared_ptr<Profiler> profiler,
							 std::shared_ptr<IEventDispatcher> eventDispatcher,
							 std::shared_ptr<ProjectManager> projectManager)
		: m_Profiler(profiler)
		, m_EventDispatcher(eventDispatcher)
		, m_ProjectManager(projectManager)
	{}

	void RuntimeLayer::onAttach() {
		m_ProjectManager->OpenProject(RUNTIME_PROJECTFILE_PATH);
		m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_ProjectManager->GetMutableRuntimeRenderSettings()));

		// Create a framebuffer to bind the texture to
		glGenFramebuffers(1, &m_Framebuffer);
	}

	void RuntimeLayer::onDetach() {
		if (m_Framebuffer) {
			glDeleteFramebuffers(1, &m_Framebuffer);
		}
	}

	void RuntimeLayer::onUpdate() {
		if (m_CurrentFrame) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer);
			glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_CurrentFrame->GetID(), 0);
			
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Bind the default framebuffer (screen)
			
			// Copy the texture to the screen
			glBlitFramebuffer(0, 0, m_CurrentFrame->GetDimensions().x, m_CurrentFrame->GetDimensions().y,
							 0, 0, m_WindowWidth, m_WindowHeight,
							 GL_COLOR_BUFFER_BIT, GL_NEAREST);
			
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Unbind
		}
	}

	void RuntimeLayer::onEvent(std::shared_ptr<IEvent> event) {
		if (event->GetType() == EventType::NEW_FRAME_RENDERED_EVENT) {
			m_CurrentFrame = std::dynamic_pointer_cast<NewFrameRenderedEvent>(event)->frame;
		}
	}
}