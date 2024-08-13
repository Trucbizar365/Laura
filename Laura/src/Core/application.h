#pragma once

#include "lrpch.h"

#include <imgui.h>

// THESE NEED TO GO ASAP // 
#include <GL/glew.h>
#include "Platform/OpenGL/OpenGLdebugFuncs.h"
///////////////////////////

#include "Core/IWindow.h"
#include "Core/LayerStack.h"
#include "Core/ImGuiLayer.h"
#include "Events/Events.h"
#include "Platform/windows/GLFWWindow.h"

#include "Renderer/Renderer.h"

#include "Components/Camera.h"
#include "Entity/Environment.h"

namespace Laura {

    class Application
    {
    public:
        Application();
        virtual ~Application();
        void run();
    protected:
        std::shared_ptr<IWindow> m_Window;
        std::shared_ptr<LayerStack> m_LayerStack;
        std::shared_ptr <ImGuiLayer> m_ImGuiLayer;
        std::shared_ptr<Renderer> m_Renderer;
    protected:
        virtual void init();
        virtual void render();
        virtual void shutdown();
    };

    Application* CreateApplication();
}

