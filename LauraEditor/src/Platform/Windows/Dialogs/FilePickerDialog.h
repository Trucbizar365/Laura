#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>

#if defined(_WIN32)
#include <windows.h>
#include <commdlg.h> // OPENFILENAME{A,W}
#include <format>
#endif

namespace Laura
{

#if defined(_WIN32)
using NativeWindowHandle = HWND;
#else
using NativeWindowHandle = void*; // Ignored on Linux
#endif

inline std::filesystem::path FilePickerDialog(
    const char* ext,
    const char* title,
    NativeWindowHandle owner = nullptr
) {
#if defined(_WIN32)

    char buff[MAX_PATH] = {};

    std::string filter = std::format("{} Files", ext);
    filter.push_back('\0');
    filter += std::format("*.{}", ext);
    filter.push_back('\0');
    filter.push_back('\0');

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = reinterpret_cast<HWND>(owner); // cast to HWND
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrTitle  = title;
    ofn.nMaxFile    = static_cast<DWORD>(sizeof(buff));
    ofn.lpstrFile   = buff;
    ofn.Flags       = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn))
        return std::filesystem::path(buff);

    return {};

#elif defined(__linux__)

    // Build Zenity command
    std::string cmd = "zenity --file-selection --title=\"";
    cmd += title ? title : "Select File";
    cmd += "\"";

    if (ext && *ext) {
        std::string pattern = ext;
        if (pattern.rfind("*.") != 0) {
            if (pattern[0] == '.') pattern = "*" + pattern;
            else pattern = "*." + pattern;
        }
        cmd += " --file-filter=\"*";
        cmd += pattern.substr(1);
        cmd += "\"";
    }

    cmd += " 2>/dev/null"; // suppress warnings

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};

    char buffer[4096];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe)) result += buffer;
    pclose(pipe);

    if (!result.empty() && result.back() == '\n') result.pop_back();
    return result.empty() ? std::filesystem::path{} : std::filesystem::path(result);

#else
#error "FilePickerDialog not implemented for this platform"
#endif
}

inline std::filesystem::path SaveFileDialog(
    const char* ext,
    const char* title,
    const char* defaultName = nullptr,
    NativeWindowHandle owner = nullptr
) {
#if defined(_WIN32)

    const int MAX_UNICODE_PATH = 32768;
    std::vector<wchar_t> buff(MAX_UNICODE_PATH, L'\0');

    // Build filter string in Unicode
    std::string filterStr;
    if (ext && *ext) {
        std::string extension = ext;
        if (extension.find("*.") != 0) {
            if (extension[0] == '.') extension = "*" + extension;
            else extension = "*." + extension;
        }
        filterStr = std::format("{} Files", ext);
        filterStr.push_back('\0');
        filterStr += extension;
        filterStr.push_back('\0');
    } else {
        filterStr = "All Files";
        filterStr.push_back('\0');
        filterStr += "*.*";
        filterStr.push_back('\0');
    }
    filterStr.push_back('\0'); // double null

    int filterSize = MultiByteToWideChar(CP_UTF8, 0, filterStr.c_str(), -1, nullptr, 0);
    std::vector<wchar_t> wfilter(filterSize > 0 ? filterSize : 1, L'\0');
    if (filterSize > 0) {
        MultiByteToWideChar(CP_UTF8, 0, filterStr.c_str(), -1, wfilter.data(), filterSize);
    }

    std::vector<wchar_t> wtitle;
    if (title && *title) {
        int titleSize = MultiByteToWideChar(CP_UTF8, 0, title, -1, nullptr, 0);
        if (titleSize > 0) {
            wtitle.resize(titleSize, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle.data(), titleSize);
        }
    }

    if (defaultName && *defaultName) {
        int nameSize = MultiByteToWideChar(CP_UTF8, 0, defaultName, -1, nullptr, 0);
        if (nameSize > 0 && nameSize < buff.size()) {
            std::vector<wchar_t> wdefaultName(nameSize, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, defaultName, -1, wdefaultName.data(), nameSize);
            std::copy(wdefaultName.begin(), wdefaultName.end(), buff.begin());
        }
    }

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = reinterpret_cast<HWND>(owner); // cast to HWND
    ofn.lpstrFilter = wfilter.data();
    ofn.lpstrTitle  = wtitle.empty() ? nullptr : wtitle.data();
    ofn.nMaxFile    = static_cast<DWORD>(buff.size());
    ofn.lpstrFile   = buff.data();
    ofn.Flags       = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

    if (GetSaveFileNameW(&ofn)) {
        return std::filesystem::path(buff.data());
    }
    return {};

#elif defined(__linux__)

    // Build Zenity command
    std::string cmd = "zenity --file-selection --save --confirm-overwrite --title=\"";
    cmd += title ? title : "Save File";
    cmd += "\"";

    if (ext && *ext) {
        std::string pattern = ext;
        if (pattern.rfind("*.") != 0) {
            if (pattern[0] == '.') pattern = "*" + pattern;
            else pattern = "*." + pattern;
        }
        cmd += " --file-filter=\"*";
        cmd += pattern.substr(1);
        cmd += "\"";
    }

    if (defaultName && *defaultName) {
        cmd += " --filename=\"";
        cmd += defaultName;
        cmd += "\"";
    }

    cmd += " 2>/dev/null"; // suppress warnings

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};

    char buffer[4096];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe)) result += buffer;
    pclose(pipe);

    if (!result.empty() && result.back() == '\n') result.pop_back();
    return result.empty() ? std::filesystem::path{} : std::filesystem::path(result);

#else
#error "SaveFileDialog not implemented for this platform"
#endif
}

}
