#include <GL/glew.h>
#include "platform/windows/GLFWWindow.h"
#include <iostream>
#include "core/KeyCodes.h"

GLFWWindowIMPL::GLFWWindowIMPL(const WindowProps& windowProps)
	: m_WindowProps(windowProps)
{
	if (!glfwInit())
	{
		glfwTerminate();
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_NativeWindow = glfwCreateWindow(m_WindowProps.width, m_WindowProps.height, (m_WindowProps.title).c_str(), NULL, NULL);
	if (!m_NativeWindow)
	{
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_NativeWindow);
	std::cout << glGetString(GL_VERSION) << std::endl;
	glfwSwapInterval(m_WindowProps.VSync);
	if (glewInit() != GLEW_OK)
	{
		if (!m_NativeWindow)
		{
			glfwDestroyWindow(m_NativeWindow);
			glfwTerminate();
		}
	}

	isRunning = true;

	glfwSetWindowUserPointer(m_NativeWindow, this);
	glfwSetKeyCallback(m_NativeWindow, GLFWKeyCallback);
	glfwSetMouseButtonCallback(m_NativeWindow, GLFWMouseButtonCallback);
	glfwSetCursorPosCallback(m_NativeWindow, GLFWMousePositionCallback);
	glfwSetScrollCallback(m_NativeWindow, GLFWScrollCallback);
}

GLFWWindowIMPL::~GLFWWindowIMPL()
{
	glfwTerminate();
}

int GLFWWindowIMPL::getWidth() const
{ 
	return m_WindowProps.width; 
}

int GLFWWindowIMPL::getHeight() const
{ 
	return m_WindowProps.height; 
}

bool GLFWWindowIMPL::isVSync() const
{ 
	return m_WindowProps.VSync;
}

void GLFWWindowIMPL::setVSync(bool enabled)
{
	m_WindowProps.VSync = enabled;
	glfwSwapInterval(m_WindowProps.VSync);
}

void GLFWWindowIMPL::onUpdate()
{
	if (glfwWindowShouldClose(m_NativeWindow))
	{
		isRunning = false;
	}
	glfwPollEvents();
	glfwSwapBuffers(m_NativeWindow);
}

void* GLFWWindowIMPL::getNativeWindow() const
{ 
	return m_NativeWindow; 
}

void GLFWWindowIMPL::setEventCallback(const std::function<void(Event*)>& callback)
{ 
	notifyDispatcher = callback;
}


#define thisWindow static_cast<GLFWWindowIMPL*>(glfwGetWindowUserPointer(window))

void GLFWWindowIMPL::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyEvent event;

	switch (action)
	{
		case GLFW_PRESS:
			event.type = EventType::KEY_PRESS_EVENT;
			break;
		case GLFW_RELEASE:
			event.type = EventType::KEY_RELEASE_EVENT;
			break;
		case GLFW_REPEAT:
			event.type = EventType::KEY_REPEAT_EVENT;
			break;	
	}

	event.key = static_cast<KeyCode>(key);
	event.keyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
	event.keyShift = (mods & GLFW_MOD_SHIFT) != 0;
	event.keyAlt = (mods & GLFW_MOD_ALT) != 0;
	event.keySuper = (mods & GLFW_MOD_SUPER) != 0;

    thisWindow->notifyDispatcher(&event);
}


void GLFWWindowIMPL::GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    MouseMoveEvent event;
    event.type = EventType::MOUSE_MOVE_EVENT;
    event.xpos = xpos;
    event.ypos = ypos;
    thisWindow->notifyDispatcher(&event);
}


void GLFWWindowIMPL::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    MouseButtonEvent event;
    event.type = (action == GLFW_PRESS) ? EventType::MOUSE_BUTTON_PRESS_EVENT : EventType::MOUSE_BUTTON_RELEASE_EVENT;
    event.button = button;
	thisWindow->notifyDispatcher(&event);
}


void GLFWWindowIMPL::GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseScrollEvent event;
    event.type = EventType::MOUSE_SCROLL_EVENT;
    event.xoffset = xoffset;
    event.yoffset = yoffset;
	thisWindow->notifyDispatcher(&event);
}

bool GLFWWindowIMPL::isKeyPressed(KeyCode key)
{
	auto state = glfwGetKey(m_NativeWindow, key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool GLFWWindowIMPL::isMouseButtonPressed(MouseCode button)
{
	auto state = glfwGetMouseButton(m_NativeWindow, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> GLFWWindowIMPL::getMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(m_NativeWindow, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}