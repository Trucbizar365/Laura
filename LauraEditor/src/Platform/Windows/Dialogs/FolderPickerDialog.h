#pragma once

#include <filesystem>
#include <string>

namespace Laura
{

#if defined(_WIN32)
#include <windows.h>
#include <shobjidl.h> // IFileDialog
using NativeWindowHandle = HWND;
#else
using NativeWindowHandle = void*; // Ignored on Linux
#endif

inline std::filesystem::path FolderPickerDialog(
    const std::string& title = "Select Folder",
    NativeWindowHandle owner = nullptr
) {
#if defined(_WIN32)
    IFileDialog* pDialog = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
                                  IID_IFileDialog, reinterpret_cast<void**>(&pDialog));
    if (FAILED(hr) || !pDialog) {
        return {};
    }

    // Tell the dialog to pick folders
    DWORD options;
    pDialog->GetOptions(&options);
    pDialog->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

    // Set title
    std::wstring wtitle(title.begin(), title.end());
    pDialog->SetTitle(wtitle.c_str());

    // Show the dialog, passing owner
    hr = pDialog->Show(reinterpret_cast<HWND>(owner)); // cast to HWND
    if (FAILED(hr)) {
        pDialog->Release();
        return {};
    }

    // Get the result
    IShellItem* pItem = nullptr;
    hr = pDialog->GetResult(&pItem);
    if (FAILED(hr) || !pItem) {
        pDialog->Release();
        return {};
    }

    PWSTR pszPath = nullptr;
    std::filesystem::path result{};
    if (SUCCEEDED(hr)) {
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
        if (SUCCEEDED(hr) && pszPath) {
            result = std::filesystem::path(pszPath);
            CoTaskMemFree(pszPath);
        }
    }

    pItem->Release();
    pDialog->Release();
    return result;

#elif defined(__linux__)
    std::string cmd = "zenity --file-selection --directory --title=\"" + title + "\" 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};

    char buffer[4096];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }
    pclose(pipe);

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result.empty() ? std::filesystem::path{} : std::filesystem::path(result);

#else
    #error "FolderPickerDialog not implemented on this platform"
#endif
}

}
