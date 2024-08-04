#include <Laura.h>

class Sandbox : public Laura::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Laura::Application* Laura::CreateApplication()
{
	return new Sandbox();
}