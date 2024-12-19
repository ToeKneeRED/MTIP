#pragma once

#include <vector>
#include <filesystem>
#include <iostream>
#include <string>

#include "Log.h"

namespace Path
{
    constexpr auto kLauncherDllPath = "\\common\\LauncherDll.dll";
} // namespace Path

struct Button
{
    Button() = default;
    Button(const Button&) = default;
    ~Button() = default;
    Button& operator=(const Button& other) = default;
    Button(std::wstring aText, DWORD aProcessId, HWND aHandle, HWND aWindowHandle)
        : text(std::move(aText))
        , processId(aProcessId)
        , handle(aHandle)
        , windowHandle(aWindowHandle)
    {
    }
    Button(Button&& aOther) noexcept
        : processId(aOther.processId)
        , handle(aOther.handle)
    {
        aOther.processId = 0;
        aOther.handle = nullptr;
    }
    Button& operator=(Button&& other) noexcept
    {
        if (this != &other)
        {
            processId = other.processId;
            handle = other.handle;

            other.processId = 0;
            other.handle = nullptr;
        }
        return *this;
    }
    bool operator==(const Button& other) const { return processId == other.processId && handle == other.handle; }
    bool operator!=(const Button& other) const { return !(*this == other); }

    std::wstring text{};
    DWORD processId = NULL;
    HWND handle = nullptr;
    HWND windowHandle = nullptr;
};


BOOL ResizeButton(const std::vector<Button>::iterator& acIterator, const std::wstring& acWideText);
LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
BOOL InjectDLL(DWORD aProcessId, LPCSTR apDllPath);

HHOOK keyboardHook;
HWND windowHandle{};
std::vector<Button> buttons{};

HFONT buttonFont = CreateFont(
    22,                  // Height of the font
    0,                   // Width of the font (0 for default)
    0,                   // Angle of escapement
    0,                   // Base-line orientation angle
    FW_DEMIBOLD,         // Font weight
    FALSE,               // Italic
    FALSE,               // Underline
    FALSE,               // Strikeout
    DEFAULT_CHARSET,     // Character set identifier
    OUT_DEFAULT_PRECIS,  // Output precision
    CLIP_DEFAULT_PRECIS, // Clipping precision
    DEFAULT_QUALITY,     // Output quality
    DEFAULT_QUALITY,     // Pitch and family
    "Coolvetica Regular" // Font name
);
HFONT titleFont = CreateFont(
    32,                  // Height of the font
    0,                   // Width of the font (0 for default)
    0,                   // Angle of escapement
    0,                   // Base-line orientation angle
    FW_BOLD,             // Font weight (bold)
    FALSE,               // Italic
    FALSE,               // Underline
    FALSE,               // Strikeout
    DEFAULT_CHARSET,     // Character set identifier
    OUT_DEFAULT_PRECIS,  // Output precision
    CLIP_DEFAULT_PRECIS, // Clipping precision
    DEFAULT_QUALITY,     // Output quality
    DEFAULT_QUALITY,     // Pitch and family
    "Coolvetica Regular" // Font name
);

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    DWORD dwProcessId;
    wchar_t text[255]{'\0'};

    GetWindowThreadProcessId(hWnd, &dwProcessId);

    auto iter = std::find_if(
        buttons.begin(), buttons.end(),
        [&](const Button& acButton)
        { return acButton.windowHandle == hWnd || acButton.processId == dwProcessId || acButton.handle == hWnd; });

    if (!hWnd || !IsWindowVisible(hWnd) || iter != buttons.end() || !GetWindowTextW(hWnd, text, sizeof(text)))
        return TRUE;

    std::wstring wideText(text);

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideText[0], (int)wideText.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wideText[0], (int)wideText.size(), &strTo[0], size_needed, NULL, NULL);

    if (ResizeButton(iter, wideText))
        return TRUE;

    HDC hdc = GetDC(windowHandle);
    SIZE textSize;
    GetTextExtentPoint32W(hdc, wideText.c_str(), static_cast<int>(wideText.length()), &textSize);
    ReleaseDC(windowHandle, hdc);

    const int& padding = 15;
    const int& buttonWidth = textSize.cx + padding * 2;
    const int& buttonHeight = textSize.cy + padding;

    RECT rcClient;
    GetClientRect(windowHandle, &rcClient);

    const int& x = (rcClient.right - buttonWidth) / 2;
    const int& y = static_cast<int>(buttons.size()) * (buttonHeight + 10) + padding;

    HWND buttonHandle = CreateWindowW(
        L"BUTTON", wideText.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, x, y, buttonWidth,
        buttonHeight, windowHandle, (HMENU)dwProcessId, (HINSTANCE)GetWindowLongPtr(windowHandle, GWLP_HINSTANCE),
        NULL);

    if (!buttonHandle)
        std::wcout << L"Button creation failed: " << GetLastError() << L"\n";

    Button* newButton = new Button{wideText, dwProcessId, buttonHandle, hWnd};
    buttons.emplace_back(*newButton);

    return TRUE;
}

void WindowCheck()
{
    for (auto iter = buttons.begin(); iter != buttons.end();)
    {
        if (!IsWindowVisible(iter->windowHandle))
        {
            DestroyWindow(iter->handle);
            iter = buttons.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    EnumWindows(EnumWindowsProc, NULL);
}