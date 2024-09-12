#include "ViewportPanel.h"
#include <IconsFontAwesome6.h>
namespace Laura
{
	void ViewportPanel::OnImGuiRender(std::shared_ptr<IImage2D> image, EditorState* editorState)
	{
		static ImGuiWindowFlags ViewportFlags = ImGuiWindowFlags_NoScrollbar |
												ImGuiWindowFlags_NoTitleBar | 
												ImGuiWindowFlags_NoCollapse |
												ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // remove the border padding
		ImGui::Begin("ViewportPanel", nullptr, ViewportFlags);
		
		ForceUpdate = false;

		int MenuHeight = DrawRenderSettingsMenu(editorState);
		DrawViewportSettingsPanel(editorState);

		if (image == nullptr)
		{
			ImGui::Text("No image to display");
			ImGui::PopStyleVar();
			ImGui::End();
			return;
		}
		
		ImageDimensions = image->GetDimensions();
		WindowDimensions = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		TLWindowPosition = glm::ivec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + MenuHeight);

		bool DimensionsChanged = (ImageDimensions != m_PrevImageDimensions || WindowDimensions != m_PrevWindowDimensions);
		bool PositionChanged = (TLWindowPosition != m_PrevWindowPosition);

		if (editorState->viewportMode == ViewportMode::CENTERED)
		{
			if (DimensionsChanged || PositionChanged || ForceUpdate)
			{
				glm::ivec2 OffsetTopLeftCorner = (WindowDimensions - ImageDimensions) / 2;
				m_TopLeftImageCoords = OffsetTopLeftCorner + TLWindowPosition;
				m_BottomRightImageCoords = OffsetTopLeftCorner + TLWindowPosition + ImageDimensions;
			}
		}

		if (editorState->viewportMode == ViewportMode::STRETCH_TO_VIEWPORT)
		{
			if (DimensionsChanged || PositionChanged || ForceUpdate)
			{
				m_TopLeftImageCoords = TLWindowPosition;
				m_BottomRightImageCoords = TLWindowPosition + WindowDimensions;
			}
		}

		if (editorState->viewportMode == ViewportMode::FIT_TO_VIEWPORT)
		{
			// if the ViewportPanel has been resized or the renderer output image size has been changed

			if (DimensionsChanged || PositionChanged || ForceUpdate)
			{
				if (DimensionsChanged || ForceUpdate)
				{
					m_PrevWindowDimensions = WindowDimensions;
					m_PrevImageDimensions = ImageDimensions;

					float WindowAspectRatio = (float)WindowDimensions.x / (float)WindowDimensions.y;
					float ImageAspectRatio = (float)ImageDimensions.x / (float)ImageDimensions.y;
					// if true width is the limiting factor (spans the entire width)
					if (WindowAspectRatio <= ImageAspectRatio)
					{
						m_TargetImageDimensions.x = WindowDimensions.x;
						m_TargetImageDimensions.y = ceil(WindowDimensions.x / ImageAspectRatio);
					}
					else // height is the limiting factor (spans the entire height)
					{
						m_TargetImageDimensions.x = ceil(WindowDimensions.y * ImageAspectRatio);
						m_TargetImageDimensions.y = WindowDimensions.y;
					}
				}

				m_PrevWindowPosition = TLWindowPosition;

				m_TopLeftImageCoords.x = (WindowDimensions.x - m_TargetImageDimensions.x) / 2.0f;
				m_TopLeftImageCoords.y = (WindowDimensions.y - m_TargetImageDimensions.y) / 2.0f;

				// offset by the viewport panel's position
				m_TopLeftImageCoords.x += TLWindowPosition.x;
				m_TopLeftImageCoords.y += TLWindowPosition.y;

				m_BottomRightImageCoords.x = m_TopLeftImageCoords.x + m_TargetImageDimensions.x;
				m_BottomRightImageCoords.y = m_TopLeftImageCoords.y + m_TargetImageDimensions.y;
			}
		}

		// ImGui Handles scaling up the texture			
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 TLImVec = ImVec2(m_TopLeftImageCoords.x, m_TopLeftImageCoords.y);
		ImVec2 BRImVec = ImVec2(m_BottomRightImageCoords.x, m_BottomRightImageCoords.y);
		drawList->AddImage((ImTextureID)image->GetID(), TLImVec, BRImVec, { 0, 1 }, { 1, 0 });

		ImGui::PopStyleVar();
		ImGui::End();
	}

	int Laura::ViewportPanel::DrawRenderSettingsMenu(EditorState* editorState)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		// Directly set FramePadding
		style.FramePadding = ImVec2(0.0f, 6.0f);
		ImGui::BeginMenuBar();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.35f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Viewport");

		float buttonWidth = ImGui::CalcTextSize(ICON_FA_GEAR).x + ImGui::GetStyle().FramePadding.x * 2;
		float windowWidth = ImGui::GetWindowWidth();
		float availableWidth = windowWidth - buttonWidth - ImGui::GetStyle().ItemSpacing.x;
			
		ImGui::SameLine(availableWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 5));
		if (ImGui::Button(ICON_FA_GEAR))
		{
			editorState->ViewportSettingsPanelOpen = true;
		}
		static int MenuHeight = ImGui::GetFrameHeight();
		ImGui::PopStyleVar();
		ImGui::EndMenuBar();
		ImGui::PopStyleColor(3);
		return MenuHeight;
	}

	void ViewportPanel::DrawViewportSettingsPanel(EditorState* editorState)
	{
		if (!editorState->ViewportSettingsPanelOpen)
			return;

		static ImGuiWindowFlags ViewportSettingsFlags = ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoCollapse;

		ImGui::Begin("Viewport Settings", &editorState->ViewportSettingsPanelOpen, ViewportSettingsFlags);
		ImGui::Text("Viewport Mode");
		
		if (
		ImGui::RadioButton("Centered", (int*)&editorState->viewportMode, (int)ViewportMode::CENTERED) ||
		ImGui::RadioButton("Stretch to Viewport", (int*)&editorState->viewportMode, (int)ViewportMode::STRETCH_TO_VIEWPORT) ||
		ImGui::RadioButton("Fit to Viewport", (int*)&editorState->viewportMode, (int)ViewportMode::FIT_TO_VIEWPORT)
		){
			ForceUpdate = true;
		}

		ImGui::End();
	}
}
