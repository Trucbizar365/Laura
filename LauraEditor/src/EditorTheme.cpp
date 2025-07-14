#include "EditorTheme.h"
#include <fstream>

namespace Laura
{
    std::pair<bool, std::string> EditorTheme::SerializeToYAML(const std::filesystem::path& filepath) {
        if (filepath.extension() != EDITOR_THEME_FILE_EXTENSION) {
            return { false, "Invalid file extension for theme file (or none selected): " + filepath.string() };
        }

        std::ofstream fout(filepath);
        if (!fout.is_open()) {
            return { false, "Could not open file for writing: " + filepath.string() };
        }

        YAML::Node node;
        try {
            node = YAML::convert<EditorTheme>::encode(*this);
        }
        catch (const YAML::RepresentationException& e) {
            return { false, "YAML representation error (make sure the file is valid): " + filepath.string() + ", error: " + e.what() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while saving file: " + filepath.string() + ", error: " + e.what() };
        }

        fout << node;
        fout.close();
        return { true, "" };
    }

    std::pair<bool, std::string> EditorTheme::DeserializeFromYAML(const std::filesystem::path& filepath) {
        if (filepath.extension() != EDITOR_THEME_FILE_EXTENSION) {
            return { false, "Invalid file extension for theme file (or none selected): " + filepath.string() };
        }

        YAML::Node node;
        try {
            node = YAML::LoadFile(filepath.string());
        }
        catch (const YAML::BadFile& e) {
            return { false, "Failed to load file: " + filepath.string() };
        }
        catch (const YAML::ParserException& e) {
            return { false, "Failed to parse file: " + filepath.string() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while loading file: " + filepath.string() + ", error: " + e.what() };
        }

        EditorTheme newEditorTheme;
        try {
            YAML::convert<EditorTheme>::decode(node, newEditorTheme);
        }
        catch (const YAML::RepresentationException& e) {
            return { false, "YAML representation error (make sure the file is valid): " + filepath.string() + ", error: " + e.what() };
        }
        catch (const std::exception& e) {
            return { false, "Unknown error occurred while loading file: " + filepath.string() + ", error: " + e.what() };
        }
        
        this->colorMap = std::move(newEditorTheme.colorMap);
        this->m_ColorPallete = std::move(newEditorTheme.m_ColorPallete);
        // keep the stack intact 

        return { true, "" };
    }

    void EditorTheme::LoadDefaultDark() {
        m_ColorPallete[EditorCol_Primary1]    = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};      // HeaderHovered, ButtonHovered
        m_ColorPallete[EditorCol_Primary2]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};   // HeaderActive, ButtonActive, FrameBgActive
        m_ColorPallete[EditorCol_Primary3]    = ImVec4{0.01f, 0.01f, 0.01f, 1.0f};      // MenuBarBg
        m_ColorPallete[EditorCol_Secondary1]  = ImVec4{0.078f, 0.078f, 0.078f, 1.0f};   // Header, TabHovered, TabSelected, AddComponentButton
        m_ColorPallete[EditorCol_Secondary2]  = ImVec4{0.138f, 0.138f, 0.138f, 1.0f};   // SelectedHeader
        m_ColorPallete[EditorCol_Accent1]     = ImVec4{0.26f, 0.59f, 0.98f, 1.00f};     // SliderGrabActive, TabDimmedSelectedOverline (blue)
        m_ColorPallete[EditorCol_Accent2]     = ImVec4{0.391f, 0.391f, 0.391f, 1.0f};   // CheckMark, SliderGrab
        m_ColorPallete[EditorCol_Text1]       = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};         // Text
        m_ColorPallete[EditorCol_Text2]       = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};         // TextDisabled
        m_ColorPallete[EditorCol_Background1] = ImVec4{0.109f, 0.109f, 0.109f, 1.0f};   // WindowBg, TransformButtonBg (X/Y/Z)
        m_ColorPallete[EditorCol_Background2] = ImVec4{0.08f, 0.08f, 0.08f, 0.940f};    // PopupBg
        m_ColorPallete[EditorCol_Error]       = ImVec4{0.609f, 0.109f, 0.109f, 1.0f};   // TransformButtonXActive
        m_ColorPallete[EditorCol_Warning]     = ImVec4{1.0f, 0.5f, 0.0f, 1.0f};         // Custom fallback
        m_ColorPallete[EditorCol_Success]     = ImVec4{0.109f, 0.609f, 0.109f, 1.0f};   // TransformButtonYActive
        m_ColorPallete[EditorCol_X]           = ImVec4{0.409f, 0.109f, 0.109f, 1.0f};   // TransformButtonXHovered
        m_ColorPallete[EditorCol_Y]           = ImVec4{0.109f, 0.409f, 0.109f, 1.0f};   // TransformButtonYHovered
        m_ColorPallete[EditorCol_Z]           = ImVec4{0.109f, 0.109f, 0.409f, 1.0f};   // TransformButtonZHovered
        ApplyAllToImgui();
    }

    void EditorTheme::LoadDefaultLight() {
        m_ColorPallete[EditorCol_Primary1]    = ImVec4{0.85f, 0.85f, 0.86f, 1.0f};       // HeaderHovered, ButtonHovered
        m_ColorPallete[EditorCol_Primary2]    = ImVec4{0.75f, 0.75f, 0.76f, 1.0f};       // HeaderActive, ButtonActive, FrameBgActive
        m_ColorPallete[EditorCol_Primary3]    = ImVec4{0.9f, 0.9f, 0.9f, 1.0f};          // MenuBarBg
        m_ColorPallete[EditorCol_Secondary1]  = ImVec4{0.93f, 0.93f, 0.93f, 1.0f};       // Header, TabHovered, TabSelected, AddComponentButton
        m_ColorPallete[EditorCol_Secondary2]  = ImVec4{0.82f, 0.82f, 0.82f, 1.0f};       // SelectedHeader
        m_ColorPallete[EditorCol_Accent1]     = ImVec4{0.2f, 0.6f, 0.15f, 1.0f};         // SliderGrabActive (greenish)
        m_ColorPallete[EditorCol_Accent2]     = ImVec4{0.3f, 0.3f, 0.3f, 1.0f};          // CheckMark, SliderGrab
        m_ColorPallete[EditorCol_Text1]       = ImVec4{0.0f, 0.0f, 0.0f, 1.0f};          // Text (black)
        m_ColorPallete[EditorCol_Text2]       = ImVec4{0.4f, 0.4f, 0.4f, 1.0f};          // TextDisabled
        m_ColorPallete[EditorCol_Background1] = ImVec4{0.98f, 0.98f, 0.98f, 1.0f};       // WindowBg
        m_ColorPallete[EditorCol_Background2] = ImVec4{0.95f, 0.95f, 0.95f, 1.0f};       // PopupBg
        m_ColorPallete[EditorCol_Error]       = ImVec4{0.8f, 0.2f, 0.2f, 1.0f};          // TransformButtonXActive
        m_ColorPallete[EditorCol_Warning]     = ImVec4{1.0f, 0.65f, 0.0f, 1.0f};         // Orange
        m_ColorPallete[EditorCol_Success]     = ImVec4{0.2f, 0.6f, 0.2f, 1.0f};          // Green
        m_ColorPallete[EditorCol_X]           = ImVec4{0.8f, 0.2f, 0.2f, 1.0f};          // TransformButtonXHovered
        m_ColorPallete[EditorCol_Y]           = ImVec4{0.2f, 0.8f, 0.2f, 1.0f};          // TransformButtonYHovered
        m_ColorPallete[EditorCol_Z]           = ImVec4{0.2f, 0.2f, 0.8f, 1.0f};          // TransformButtonZHovered
        ApplyAllToImgui();
    }

    void EditorTheme::ApplyAllToImgui() {
        // populate colormap
        colorMap[ImGuiCol_WindowBg]                  = EditorCol_Background1;
        colorMap[ImGuiCol_PopupBg]                   = EditorCol_Background2;
        colorMap[ImGuiCol_Border]                    = EditorCol_Secondary2;
        colorMap[ImGuiCol_Header]                    = EditorCol_Secondary1;
        colorMap[ImGuiCol_HeaderHovered]             = EditorCol_Primary1;
        colorMap[ImGuiCol_HeaderActive]              = EditorCol_Primary2;
        colorMap[ImGuiCol_Button]                    = EditorCol_Secondary1;
        colorMap[ImGuiCol_ButtonHovered]             = EditorCol_Primary1;
        colorMap[ImGuiCol_ButtonActive]              = EditorCol_Primary2;
        colorMap[ImGuiCol_CheckMark]                 = EditorCol_Accent2;
        colorMap[ImGuiCol_SliderGrab]                = EditorCol_Accent2;
        colorMap[ImGuiCol_SliderGrabActive]          = EditorCol_Accent1;
        colorMap[ImGuiCol_FrameBg]                   = EditorCol_Background1;
        colorMap[ImGuiCol_FrameBgHovered]            = EditorCol_Background1;
        colorMap[ImGuiCol_FrameBgActive]             = EditorCol_Primary2;
        colorMap[ImGuiCol_Tab]                       = EditorCol_Background2;
        colorMap[ImGuiCol_TabHovered]                = EditorCol_Secondary2;
        colorMap[ImGuiCol_TabActive]                 = EditorCol_Secondary2;
        colorMap[ImGuiCol_TabSelectedOverline]       = EditorCol_Accent1;
        colorMap[ImGuiCol_TabDimmedSelectedOverline] = EditorCol_Primary1;
        colorMap[ImGuiCol_TabUnfocused]              = EditorCol_Secondary2;
        colorMap[ImGuiCol_TabUnfocusedActive]        = EditorCol_Secondary2;
        colorMap[ImGuiCol_TitleBg]                   = EditorCol_Background2;
        colorMap[ImGuiCol_TitleBgActive]             = EditorCol_Background2;
        colorMap[ImGuiCol_TitleBgCollapsed]          = EditorCol_Background2;
        colorMap[ImGuiCol_ScrollbarGrab]             = EditorCol_Secondary2;
        colorMap[ImGuiCol_ResizeGrip]                = EditorCol_Secondary2;
        colorMap[ImGuiCol_ResizeGripHovered]         = EditorCol_Secondary2;
        colorMap[ImGuiCol_ResizeGripActive]          = EditorCol_Secondary2;
        colorMap[ImGuiCol_Separator]                 = EditorCol_Secondary2;
        colorMap[ImGuiCol_SeparatorHovered]          = EditorCol_Secondary2;
        colorMap[ImGuiCol_SeparatorActive]           = EditorCol_Secondary2;
        colorMap[ImGuiCol_Text]                      = EditorCol_Text1;
        colorMap[ImGuiCol_TextDisabled]              = EditorCol_Text2;
        colorMap[ImGuiCol_MenuBarBg]                 = EditorCol_Primary3;

        // set imgui based on colormap
        for (const auto& [widget, col] : colorMap) {
			ImGui::GetStyle().Colors[widget] = m_ColorPallete[col];
        }
    }
}