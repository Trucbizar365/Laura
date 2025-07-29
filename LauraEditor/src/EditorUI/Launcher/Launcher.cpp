#include <IconsFontAwesome6.h>
#include "Launcher.h"


namespace Laura
{

	std::optional<std::filesystem::path> Launcher::ShowFolderPickerDialog() {
		IFileDialog* pDialog = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
									  IID_IFileDialog, reinterpret_cast<void**>(&pDialog));
		if (FAILED(hr) || !pDialog) {
			return std::nullopt;
		}
		// Tell the dialog to pick folders
		DWORD options;
		pDialog->GetOptions(&options);
		pDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
		pDialog->SetTitle(L"Select Project Folder");
		// Show the dialog
		hr = pDialog->Show(nullptr);
		if (FAILED(hr)) {
			pDialog->Release();
			return std::nullopt;
		}
		// Get the result
		IShellItem* pItem = nullptr;
		hr = pDialog->GetResult(&pItem);
		if (FAILED(hr) || !pItem) {
			pDialog->Release();
			return std::nullopt;
		}
		PWSTR pszPath = nullptr;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
		std::optional<std::filesystem::path> result;

		if (SUCCEEDED(hr)) {
			result = std::filesystem::path(pszPath);
			CoTaskMemFree(pszPath);
		}
		pItem->Release();
		pDialog->Release();
		return result;
	}

	void Launcher::OnImGuiRender() {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("##Launcher", nullptr, flags);

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		float buttonWidth = windowSize.x / 8.0f;
		float buttonHeight = 40.0f;
		float spacingY = 16.0f;

		// Total height of both buttons + spacing
		float totalHeight = buttonHeight * 2 + spacingY;

		// Center buttons vertically
		ImGui::SetCursorPosY((windowSize.y - totalHeight) / 2.0f);

		// Center horizontally
		float centerX = (windowSize.x - buttonWidth) / 2.0f;
		ImGui::SetCursorPosX(centerX);
		if (ImGui::Button("New Project   " ICON_FA_CUBE, ImVec2(buttonWidth, buttonHeight))) {
			auto projectRoot = ShowFolderPickerDialog();
			if (projectRoot) {
				if (std::filesystem::exists(*projectRoot) && std::filesystem::is_empty(*projectRoot)) {
					std::cout << "successfully set projectRoot" << std::endl;
				}
				else {
					std::cout << "selected nonempty folder" << std::endl;
				}
			}
			else {
				std::cout << "returned nullopt (new proj)" << std::endl;
			}
		}

		ImGui::Spacing();

		ImGui::SetCursorPosX(centerX);
		if (ImGui::Button("Open Project   " ICON_FA_SHARE, ImVec2(buttonWidth, buttonHeight))) {
			auto projectRoot = ShowFolderPickerDialog();
			if (projectRoot) {
				if (std::filesystem::exists(*projectRoot) && std::filesystem::is_directory(*projectRoot)) {
					std::cout << "Successfully opened a project" << std::endl;
				}
				else {
					std::cout << "opened invalid folder" << std::endl;
				}
			}
			else {
				std::cout << "returned nullopt (open proj)" << std::endl;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}
