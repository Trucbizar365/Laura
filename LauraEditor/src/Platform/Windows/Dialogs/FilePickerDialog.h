#pragma once

#include <filesystem>
#include <string>
#include <commdlg.h> // OPENFILENAMEA
#include <format>

namespace Laura
{

    inline std::filesystem::path FilePickerDialog(const char* ext, const char* title, HWND owner = nullptr) {
        char buff[MAX_PATH] = {};

        std::string filter = std::format("{} Files", ext);
		filter.push_back('\0');
		filter += std::format(" * {}", ext);
		filter.push_back('\0');
		filter.push_back('\0');

        OPENFILENAMEA ofn = {};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner   = owner;
        ofn.lpstrFilter = filter.c_str();
        ofn.lpstrTitle  = title;
        ofn.nMaxFile    = static_cast<DWORD>(sizeof(buff));
        ofn.lpstrFile   = buff;
        ofn.Flags       = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn)) {
            return std::filesystem::path(buff);
        }
        return {}; // empty path if cancelled
    }
}