#include "core/ImGuiContextManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <IconsFontAwesome6.h>
#include <iconsFontAwesome6Brands.h>

#include "lrpch.h"

#include "events/events.h"

namespace Laura {

    ImGuiContextManager::ImGuiContextManager(std::shared_ptr<IWindow> window)
        : m_Window(window)
    {
    }

    ImGuiContextManager::~ImGuiContextManager()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiContextManager::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
 
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // FONTS - merging Font Awesome with the default font (Icons)
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->AddFontDefault();

        ImFontConfig config;
        config.MergeMode = true;  // Merge Font Awesome with the default font
        config.PixelSnapH = true;
        config.OversampleH = 3;
        config.OversampleV = 3;
        config.GlyphMinAdvanceX = 15.0f;
        config.GlyphOffset = { 0.f, 3.f };
        static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        io.Fonts->AddFontFromFileTTF(LR_RESOURCES_PATH "Fonts/fontawesome-free-6.6.0-desktop/Font Awesome 6 Free-Solid-900.otf", 15.0f, &config, icon_ranges);

        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;


        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_Window->getNativeWindow()), true); // true: install callbacks in the GLFW window
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiContextManager::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiContextManager::EndFrame()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_Window->getWidth(), (float)m_Window->getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}
