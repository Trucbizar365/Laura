#include "EditorUI/SceneHierarchyPanel.h"
#include "EditorUI/UtilityUI.h"
#include <IconsFontAwesome6.h>

namespace Laura
{
    SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<ThemeManager> themeManager)
		: m_EditorState(editorState), m_ThemeManager(themeManager)
    {
    }

    void SceneHierarchyPanel::OnImGuiRender(std::shared_ptr<Scene> scene)
    {
        ImGui::Begin(ICON_FA_SITEMAP " Scene Hierarchy");

        if (ImGui::BeginMenu(ICON_FA_SQUARE_PLUS " Add"))
        {
            if (ImGui::MenuItem("Empty")) { scene->CreateEntity(); }
            if (ImGui::BeginMenu("3D Object"))
            {
                if (ImGui::MenuItem("Sphere")) {}
                if (ImGui::MenuItem("Cuboid")) {}
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("2D Plane")) {}
            if (ImGui::MenuItem("Camera")) {}
            if (ImGui::MenuItem("3D Mesh")) {}

            ImGui::EndMenu();
        }

        entt::registry* activeRegistry = scene->Get();
        if(!activeRegistry)
		{
			ImGui::End();
			return;
		}
        
        ImVec2 panelDims = ImGui::GetContentRegionAvail();
        float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
                                    | ImGuiTreeNodeFlags_OpenOnDoubleClick
                                    | ImGuiTreeNodeFlags_SpanAvailWidth
                                    | ImGuiTreeNodeFlags_AllowItemOverlap
                                    | ImGuiTreeNodeFlags_Framed
                                    | ImGuiTreeNodeFlags_FramePadding;

        auto view = activeRegistry->view<entt::entity>();
        for (auto entityID : view)
        {
            bool nodeOpen = false;
            // Get the TAG of the entity (used to display in the hierarchy)
            Entity entity(entityID, activeRegistry);
            std::string& tag = entity.GetComponent<TagComponent>().Tag;
            // if the entity is selected, highlight it
            if (entityID == m_EditorState->selectedEntity)
            {
                m_ThemeManager->ImGuiSet(ImGuiCol_Header, m_ThemeManager->GetActiveTheme()->SelectedHeader);
                nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)entityID, flags, tag.c_str());
                ImGui::SameLine(panelDims.x - lineHeight * 0.5);
 
                std::cout << "Testing delete entity\n";
                if(!m_EditorState->doubleConfirmDelete)
				{
                    if (ImGui::Button(ICON_FA_TRASH_CAN))
                    {
                        scene->DestroyEntity(entity);
                    }
				}
				else
				{
                    if (ImGui::Button(ICON_FA_TRASH_CAN))
                    {
                        ImGui::OpenPopup(ICON_FA_TRASH_CAN " Delete Entity");
                    }
                    // will only execute if the OpenPopup function was called
                    if (OpenConfirmationPopup(ICON_FA_TRASH_CAN " Delete Entity", "Are you sure you want to delete this entity?", m_ThemeManager))
                    {
                        scene->DestroyEntity(entity);
                    }
				}

                
                m_ThemeManager->ImGuiSet(ImGuiCol_Header, m_ThemeManager->GetActiveTheme()->DefaultHeader);
			}
			else
			{
                // display a non selected entity
				nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)entityID, flags, tag.c_str());
			}
            if (ImGui::IsItemClicked())
            {
                m_EditorState->selectedEntity = entityID;
            }
            // display any children entities (if any)
            if (nodeOpen)
            {
                // TODO: Child entity system
                ImGui::BulletText("This is a testing text.");
                ImGui::TreePop();
            }
        }

        // Deselect the selected entity if the user clicks in the window but outside of any tree node
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_EditorState->selectedEntity = entt::null;
		}

        ImGui::PopStyleVar();
        
        ImGui::End();
    }
}