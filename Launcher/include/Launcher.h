#pragma once

#include <vector>
#include <filesystem>
#include <string>

constexpr auto kLauncherDllPath = "\\common\\LauncherDll.dll";

struct Button
{
    Button() = default;
    Button(const Button&) = default;
    ~Button() = default;
    Button& operator=(const Button& other) = default;
    Button(std::wstring aText, DWORD aProcessId, HWND aHandle, HWND aWindowHandle) : text(std::move(aText)), processId(aProcessId), handle(aHandle), windowHandle(aWindowHandle){}
    Button(Button&& aOther) noexcept
        : processId(aOther.processId), handle(aOther.handle)
    {
        aOther.processId = 0;
        aOther.handle = nullptr;
    }
    Button& operator=(Button&& other) noexcept {
        if (this != &other) {
            processId = other.processId;
            handle = other.handle;

            other.processId = 0;
            other.handle = nullptr;
        }
        return *this;
    }
    bool operator==(const Button& other) const {
        return processId == other.processId && handle == other.handle;
    }
    bool operator!=(const Button& other) const {
        return !(*this == other);
    }

    std::wstring text{};
    DWORD processId = NULL;
    HWND handle = nullptr;
    HWND windowHandle = nullptr;
};

void SetupConsole() noexcept;

BOOL InjectDLL(DWORD aProcessId, LPCSTR apDllPath);

BOOL ResizeButton(const std::vector<Button>::iterator& acIterator, const std::wstring& acWideText);

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK keyboardHook;

static HWND windowHandle{};

std::vector<Button> buttons{};
std::vector<Button> visibleWindows{};

HFONT buttonFont = CreateFont(
    22,                     // Height of the font
    0,                      // Width of the font (0 for default)
    0,                      // Angle of escapement
    0,                      // Base-line orientation angle
    FW_DEMIBOLD,            // Font weight
    FALSE,                  // Italic
    FALSE,                  // Underline
    FALSE,                  // Strikeout
    DEFAULT_CHARSET,        // Character set identifier
    OUT_DEFAULT_PRECIS,     // Output precision
    CLIP_DEFAULT_PRECIS,    // Clipping precision
    DEFAULT_QUALITY,        // Output quality
    DEFAULT_QUALITY,        // Pitch and family
    "Coolvetica Regular"    // Font name
);

HFONT titleFont = CreateFont(
    32,                     // Height of the font
    0,                      // Width of the font (0 for default)
    0,                      // Angle of escapement
    0,                      // Base-line orientation angle
    FW_BOLD,                // Font weight (bold)
    FALSE,                  // Italic
    FALSE,                  // Underline
    FALSE,                  // Strikeout
    DEFAULT_CHARSET,        // Character set identifier
    OUT_DEFAULT_PRECIS,     // Output precision
    CLIP_DEFAULT_PRECIS,    // Clipping precision
    DEFAULT_QUALITY,        // Output quality
    DEFAULT_QUALITY,        // Pitch and family
    "Coolvetica Regular"    // Font name
);

