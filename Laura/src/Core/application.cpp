#include "core/application.h"

#include "Core/Log.h"
#include "renderer/ITexture2D.h"
#include "Assets/MeshLoader.h"

namespace Laura {

	Application::Application()
	{
	}

	Application::~Application(){}

	void Application::init()
	{
		Log::Init();
		m_Window = IWindow::createWindow();
		m_LayerStack = std::make_shared<LayerStack>();
		m_ImGuiLayer = std::make_shared<ImGuiLayer>(m_Window);
		// make window forward events to the layer stack
		m_Window->setEventCallback([this](Event* event) { m_LayerStack->onEvent(event); });
		m_LayerStack->PushLayer(m_ImGuiLayer);
		m_Renderer = std::make_shared<Renderer>();
	}

	void Application::run()
	{		
		init();

		//float aspectRatio = 16.0f / 9.0f;
		while (!m_Window->shouldClose())
		{
			m_Window->onUpdate();
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			m_LayerStack->onUpdate();

			m_ImGuiLayer->Begin();
			m_LayerStack->onImGuiRender();
			m_ImGuiLayer->End();
			render();
		}
		shutdown();
	}


	void Application::render()
	{
	}

	void Application::shutdown()
	{
		glfwTerminate();
	}
}