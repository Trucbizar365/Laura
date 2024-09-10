#include "EditorUI/InspectorPanel.h"

namespace Laura
{

    void InspectorPanel::OnImGuiRender(std::shared_ptr<Scene> scene, EditorState& editorState)
    {
        if (editorState.selectedEntity == entt::null)
		{
			ImGui::Begin("Inspector");
			ImGui::Text("Select an entity to inspect");
			ImGui::End();
			return;
		}

        entt::registry* activeRegistry = scene->Get();
        entt::entity selectedEntity = editorState.selectedEntity;
        Entity entity(selectedEntity, activeRegistry); // convert to Laura Entity

        ImGui::Begin("Inspector");

		/// TAG COMPONENT UI //////////////////////////////////////////////////////////////////

        if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		/// TRANSFORM COMPONENT UI ////////////////////////////////////////////////////////////
		
		if (entity.HasComponent<TransformComponent>())
		{
			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			glm::vec3 position = TransformHandler::GetTranslation(transformComponent);
			glm::vec3 rotation = TransformHandler::GetRotation(transformComponent);
			glm::vec3 scale = TransformHandler::GetScale(transformComponent);

			ImGui::SeparatorText("Transform");

			ImGui::Text("Position");
			float sliders_width = 200;
			ImGui::PushItemWidth(sliders_width / 3.0f);
			ImGui::PushID("Position");
			/// POSITION X ///
			static bool updatePosition = false;
			if (ImGui::DragFloat("X", &position.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updatePosition = true; }
			ImGui::SameLine(0, 10);
			/// POSITION Y ///
			if (ImGui::DragFloat("Y", &position.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updatePosition = true; }
			ImGui::SameLine(0, 10);
			/// POSITION Z ///
			if (ImGui::DragFloat("Z", &position.z, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updatePosition = true; }
			ImGui::PopItemWidth();
			ImGui::PopID();
			if (updatePosition)
			{
				TransformHandler::SetTranslation(transformComponent, position);
				updatePosition = false;
			}

			ImGui::Text("Rotation");
			ImGui::PushItemWidth(sliders_width / 3.0f);
			static bool updateRotation = false;
			ImGui::PushID("Rotation");
			/// ROTATION X ///
			if (ImGui::DragFloat("X", &rotation.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updateRotation = true; }
			ImGui::SameLine(0, 10);
			/// ROTATION Y ///
			if (ImGui::DragFloat("Y", &rotation.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updateRotation = true; }
			ImGui::SameLine(0, 10);
			/// ROTATION Z ///
			if (ImGui::DragFloat("Z", &rotation.z, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updateRotation = true; }
			ImGui::PopItemWidth();
			ImGui::PopID();
			if (updateRotation)
			{
				TransformHandler::SetRotation(transformComponent, rotation);
				updateRotation = false;
			}
			

			ImGui::Text("Scale");
			ImGui::PushItemWidth(sliders_width / 3.0f);
			static bool updateScale = false;
			ImGui::PushID("Scale");
			/// SCALE X ///
			if( ImGui::DragFloat("X", &scale.x, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updateScale = true; }
			ImGui::SameLine(0, 10);
			/// SCALE Y ///
			if (ImGui::DragFloat("Y", &scale.y, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updateScale = true; }
			ImGui::SameLine(0, 10);
			/// SCALE Z ///
			if (ImGui::DragFloat("Z", &scale.z, 0.05f, -FLT_MAX, FLT_MAX, "%.3f")) { updateScale = true; }
			ImGui::PopItemWidth();
			ImGui::PopID();
			if (updateScale)
			{
				TransformHandler::SetScale(transformComponent, scale);
				updateScale = false;
			}
		}

		/// CAMERA COMPONENT UI ////////////////////////////////////////////////////////////////


        ImGui::End();
    }
}