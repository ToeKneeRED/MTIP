#pragma once
#include <iostream>
#include <Windows.h>
#include "Format.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

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

    static Log& Get()
    {
        static Log s_instance;

        return s_instance;
    }

    template <typename T, typename... Args> void Print(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            // std::cout << std::format(("[{}Log{}]\t"), Text<T>::Foreground::White, Text<T>::Reset)
            //           << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;
            spdlog::info(
                "[{}Log{}]\t{}", Text<T>::Foreground::White, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            /*std::wcout << std::format((L"[{}Log{}]\t"), Text<T>::Foreground::White, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::info(
                L"[{}Log{}]\t{}", Text<T>::Foreground::White, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
    }

    template <typename T, typename... Args> void Verbose(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            /*std::cout << std::format(("[{}Verbose{}]\t"), Text<T>::Foreground::Gray, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::info(
                ("[{}Verbose{}]\t{}"), Text<T>::Foreground::Gray, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            /*std::wcout << std::format((L"[{}Verbose{}]\t"), Text<T>::Foreground::Gray, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::info(
                (L"[{}Verbose{}]\t{}"), Text<T>::Foreground::Gray, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
    }

    template <typename T, typename... Args> void Warn(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            /*std::cout << std::format(("[{}Warn{}]\t"), Text<T>::Foreground::LightYellow, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::warn(
                ("[{}Warn{}]\t{}"), Text<T>::Foreground::LightYellow, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            /*std::wcout << std::format((L"[{}Warn{}]\t"), Text<T>::Foreground::LightYellow, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::warn(
                (L"[{}Warn{}]\t{}"), Text<T>::Foreground::LightYellow, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
    }

    template <typename T, typename... Args> void Error(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            /*std::cout << std::format(("[{}Error{}]\t"), Text<T>::Foreground::LightRed, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::error(
                ("[{}Error{}]\t{}"), Text<T>::Foreground::LightRed, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            /*std::wcout << std::format((L"[{}Error{}]\t"), Text<T>::Foreground::LightRed, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::error(
                (L"[{}Error{}]\t{}"), Text<T>::Foreground::LightRed, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
    }

    template <typename T, typename... Args> void Debug(const T* acMsg, Args&&... aArgs) noexcept
    {
        if constexpr (std::is_same_v<T, char>)
        {
            /*std::cout << std::format(("[{}Debug{}]\t"), Text<T>::Foreground::Cyan, Text<T>::Reset)
                      << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::debug(
                ("[{}Debug{}]\t{}"), Text<T>::Foreground::Cyan, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
        }
        else if constexpr (std::is_same_v<T, wchar_t>)
        {
            /*std::wcout << std::format((L"[{}Debug{}]\t"), Text<T>::Foreground::Cyan, Text<T>::Reset)
                       << std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)) << std::endl;*/
            spdlog::debug(
                (L"[{}Debug{}]\t{}"), Text<T>::Foreground::Cyan, Text<T>::Reset,
                std::vformat(acMsg, std::make_format_args(std::forward<Args>(aArgs)...)));
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
        if (AllocConsole())
        {
            SetConsoleTitleA("MTIP Console");
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);

            //FILE* stdinFile;
            //FILE* stdoutFile;
            //FILE* stderrFile;

            //if (freopen_s(&stdinFile, "conin$", "r", stdin) != 0)
            //    std::cerr << "Error redirecting stdin\n";

            //if (freopen_s(&stdoutFile, "conout$", "w", stdout) != 0)
            //    std::cerr << "Error redirecting stdout\n";

            //if (freopen_s(&stderrFile, "conout$", "w", stderr) != 0)
            //    std::cerr << "Error redirecting stderr\n";

            // console output handling
            HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

            if (stdHandle == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Error: unable to get handle to stdout.\n";
                return;
            }

            DWORD dwMode = 0;

            GetConsoleMode(stdHandle, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(stdHandle, dwMode);
            if (!SetConsoleMode(stdHandle, dwMode))
                std::cerr << "Error: unable to set console mode.\n";

            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(stdHandle, &cursorInfo);
            cursorInfo.dwSize = 1;
            cursorInfo.bVisible = FALSE;
            SetConsoleCursorInfo(stdHandle, &cursorInfo);

            // console input handling
            stdHandle = GetStdHandle(STD_INPUT_HANDLE);

            if (stdHandle == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Error: unable to get handle to stdout.\n";
                return;
            }

            SetConsoleMode(stdHandle, ENABLE_EXTENDED_FLAGS);
            
            // logger
            auto narrow = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            narrow->set_level(spdlog::level::debug);
            narrow->set_pattern("[%H:%M:%S.%e] %v");

            auto m_narrowLogger = std::make_shared<spdlog::logger>("", spdlog::sinks_init_list{narrow});
            m_narrowLogger->set_level(spdlog::level::debug);

            spdlog::set_default_logger(m_narrowLogger);
        }
    }
};
