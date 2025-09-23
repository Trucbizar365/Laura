#pragma once

#include "Core/application.h"
#include "EngineCfg.h"
#include <objbase.h>

// returns new heap allocated polymorphic application
extern Laura::Application* Laura::CreateApplication(const std::filesystem::path& exeDir);

int main(int argc, char** argv) {
    // Initialize COM in STA for shell dialogs (IFileDialog) to work reliably
    HRESULT comHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    std::filesystem::path exePath = std::filesystem::weakly_canonical(argv[0]);
    std::filesystem::path exeDir = exePath.parent_path();

    Laura::EngineCfg::Init(exeDir); // init EngineCfg::RESOURCES_PATH

    Laura::Application* app = Laura::CreateApplication(exeDir);
    app->run();
    delete app;

    if (SUCCEEDED(comHr)) {
        CoUninitialize();
    }
}