#pragma once
#include <iostream>
#include <Windows.h>
#include "Format.h"
#include "spdlog/sinks/base_sink.h"

struct Log
{
    enum class Type : uint8_t
    {
        Print = 0,
        Verbose,
        Warn,
        Error,
        Debug
    };

    HANDLE ConsoleHandle;

    static Log& Get()
    {
        static Log instance;

        return instance;
    }

    template <typename T, typename... Args> void Print(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            std::cout << std::format(("[{}Log{}]\t"), Text<T>::Foreground::White, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            std::wcout << std::format((L"[{}Log{}]\t"), Text<T>::Foreground::White, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
    }

    template <typename T, typename... Args> void Verbose(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            std::cout << std::format(("[{}Verbose{}]\t"), Text<T>::Foreground::Gray, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            std::wcout << std::format((L"[{}Verbose{}]\t"), Text<T>::Foreground::Gray, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
    }

    template <typename T, typename... Args> void Warn(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            std::cout << std::format(("[{}Warn{}]\t"), Text<T>::Foreground::LightYellow, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            std::wcout << std::format((L"[{}Warn{}]\t"), Text<T>::Foreground::LightYellow, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
    }

    template <typename T, typename... Args> void Error(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            std::cout << std::format(("[{}Error{}]\t"), Text<T>::Foreground::LightRed, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            std::wcout << std::format((L"[{}Error{}]\t"), Text<T>::Foreground::LightRed, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
    }

    template <typename T, typename... Args> void Debug(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            std::cout << std::format(("[{}Debug{}]\t"), Text<T>::Foreground::Cyan, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            std::wcout << std::format((L"[{}Debug{}]\t"), Text<T>::Foreground::Cyan, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
        }
    }

    ~Log() = default;
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

private:
    Log()
    {
        AllocConsole();

        ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        FILE* stdinFile;
        FILE* stdoutFile;
        FILE* stderrFile;

        if (freopen_s(&stdinFile, "conin$", "r", stdin) != 0)
            std::cerr << "Error redirecting stdin\n";

        if (freopen_s(&stdoutFile, "conout$", "w", stdout) != 0)
            std::cerr << "Error redirecting stdout\n";

        if (freopen_s(&stderrFile, "conout$", "w", stderr) != 0)
            std::cerr << "Error redirecting stderr\n";

        if (ConsoleHandle == INVALID_HANDLE_VALUE)
        {
            std::cerr << "Error: unable to get handle to stdout.\n";
            return;
        }

        // allow ansi escape codes
        DWORD dwMode = 0;
        if (!GetConsoleMode(ConsoleHandle, &dwMode))
        {
            std::cerr << "Error: unable to get console mode.\n";
            return;
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        if (!SetConsoleMode(ConsoleHandle, dwMode))
            std::cerr << "Error: unable to set console mode.\n";

        // cursor
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(ConsoleHandle, &cursorInfo);
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(ConsoleHandle, &cursorInfo);
    }
};
