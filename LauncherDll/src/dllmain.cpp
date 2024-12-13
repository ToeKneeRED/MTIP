// dllmain.cpp : Defines the entry point for the DLL application.
#include <charconv>
#include <iostream>

#include "pch.h"

#include <string>
#include <Psapi.h>
#include <sstream>

#include "Hooks.h"
#include "Addresses.h"

void SetupConsole()
{
	AllocConsole();

	FILE* stdinFile;
	FILE* stdoutFile;
	FILE* stderrFile;

	// Redirect stdin
	if (freopen_s(&stdinFile, "conin$", "r", stdin) != 0) 
	{
		std::cerr << "Error redirecting stdin\n";
	}

	// Redirect stdout
	if (freopen_s(&stdoutFile, "conout$", "w", stdout) != 0) 
	{
		std::cerr << "Error redirecting stdout\n";
	}

	// Redirect stderr
	if (freopen_s(&stderrFile, "conout$", "w", stderr) != 0) 
	{
		std::cerr << "Error redirecting stderr\n";
	}

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		SetupConsole();

	    HWND launcherHwnd = FindWindowW(nullptr, L"Black Ops III Mod Tools Launcher");
    	SetWindowTextW(launcherHwnd, L"Toe Knee's Mod Tools Improvement Project");

    	// i think this is an attempt at a game hook lol
    	/*if (!launcherHwnd)
		{
			if (Hooks::HookWString(reinterpret_cast<void*>(0x7FF7B4075168), L"something"))
			{
				MessageBoxA(nullptr, "string hooked", "Success", MB_OK);
			}

			return FALSE;
		}*/

    	const std::string newBuildButtonText = "ToeKnee";

    	if (Hooks::HookValue<const char*>(BUILD_BUTTON_TEXT, newBuildButtonText.c_str()))
    	{
    		
    	}
    	else
    	{
    		MessageBoxA(launcherHwnd, "Failed to change build button text", "Error", MB_OK);
    	}

        Hooks::HookIAT(hModule, "MessageBoxW");
		MessageBoxW(nullptr, L"something :)", L"Hooked", MB_OK);
		if (IsDebuggerPresent())
			std::cout << "debug";

    	std::cout << "IsDebuggerPresent called";
	}
	break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
	default:
        break;
    }
    return TRUE;
}

