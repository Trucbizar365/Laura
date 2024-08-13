/////////////////////////////
/// OFFICIAL LAURA EDITOR ///
/////////////////////////////

#include <Laura.h>
#include <LauraEntrypoint.h>
#include <EditorLayer.h>

namespace Laura
{

	class LauraEditor : public Application
	{
	public:
		LauraEditor()
			: Application() // call the default application constructor
		{

		}

		void init() override
		{
			Application::init(); // call the default init method first
			m_LayerStack->PushLayer(std::make_shared<EditorLayer>(m_Renderer));
		}

		~LauraEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new LauraEditor();
	}

}