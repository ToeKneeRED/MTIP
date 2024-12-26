// dllmain.cpp : Defines the entry point for the DLL application.
#include <filesystem>
#include <fstream>
#include <print>

#include "pch.h"
#include "Hooks.h"
#include "Log.h"
#include "TKString.h"

using namespace ToeKnee;

auto& g_log = Log::Get();

void CheckLauncherBat()
{
    String batPath = std::filesystem::current_path().string().append("\\modtools_launcher.bat").c_str();

    g_log.Print("bat path: {}", batPath.c_str());

    String prefix = "start \"\" ";
    std::ifstream file(batPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Unable to open file: " + batPath);
    }

    std::vector<String> lines;
    String lineString;

    while (std::getline(file, lineString))
    {
        if (!lineString.empty())
        {
            lines.push_back(lineString);
        }
    }
    file.close();

    if (!lines.empty())
    {
        String lastLine;
        lastLine = lines.back();
        if (!lastLine.starts_with(prefix))
        {
            lastLine.insert(0, prefix);
        }

        lines.back() = lastLine;
    }

    std::ofstream outFile(batPath);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Error: Unable to write to file: " + batPath);
    }

    for (const auto& line : lines)
    {
        outFile << line << '\n';
    }

    outFile.close();
}

HWND console;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CheckLauncherBat();

        MH_STATUS status = MH_Initialize();
        if (status != MH_OK)
        {
            std::wstring asdf{L"failed minhook initialize: " + std::to_wstring(status)};
            MessageBoxW(nullptr, asdf.c_str(), L":(", MB_OK);
            break;
        }

        std::thread(
            [&]() {
            while (true)
            {
                HWND asdf = FindWindowW(nullptr, L"Black Ops III Mod Tools Launcher");
                if (asdf)
                {
                    SetWindowTextW(asdf, L"Toe Knee's Mod Tools Improvement Project");
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(750));
            }
        }).detach();

    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    default: break;
    }

    return TRUE;

}