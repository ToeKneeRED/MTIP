// Launcher.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Launcher.h"

#include <iostream>
#include <Resource.h>

constexpr auto MAX_LOADSTRING = 100;

HINSTANCE hInst;                    // current instance
CHAR szTitle[MAX_LOADSTRING];       // The title bar text
CHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY
wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#if 0
    // color test
    for (int i = 0; i < 256; i += 8)
    {
        for (int j = 0; j < 256; j += 8)
        {
            for (int k = 0; k < 256; k += 8)
            {
                std::cout << std::format("\x1B[38;2;{};{};{}m", i, j, k) << i << ";" << j << ";" << k
                    << Text<char>::Reset << "\t";
            }
        }
    }
    std::cout << std::endl;
#endif

    auto& log = Log::Get();
    log.Print("thing1");
    log.Verbose("thing2");
    log.Warn("thing3");
    log.Error("thing4");
    log.Debug("thing5");

    // Initialize global strings
    // LoadStringA(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    // LoadStringA(hInstance, IDC_LAUNCHER, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_LAUNCHER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        WindowCheck();

        if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAUNCHER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LAUNCHER);
    wcex.lpszClassName = "Launcher";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    windowHandle = CreateWindowEx(
        0L, "Launcher", "Launcher", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 800, 1000, nullptr, nullptr, hInstance,
        nullptr);

    if (!windowHandle)
    {
        auto err = GetLastError();
        std::cout << "error: " << err;
        return FALSE;
    }

    ShowWindow(windowHandle, nCmdShow);
    UpdateWindow(windowHandle);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        auto pIter = std::find_if(
            buttons.begin(), buttons.end(),
            [&](const Button& acButton) { return (int)acButton.processId == wmId || acButton.handle == hWnd; });

        if (pIter != buttons.end())
        {
            auto path = std::filesystem::current_path();

            // unpleasant but works for now
            for (int i = 0; i < 4; ++i)
            {
                if (path.has_parent_path())
                {
                    path = path.parent_path();
                }
                else
                {
                    break;
                }
            }

            std::string dllPath = path.string().append(Path::kLauncherDllPath);

            if (std::filesystem::exists(dllPath))
            {
                InjectDLL(pIter->processId, dllPath.c_str());
            }
        }

        switch (wmId)
        {
        case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
        case IDM_EXIT: DestroyWindow(hWnd); break;
        default: return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...

        /*HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);

        SelectObject(hdc, titleFont);
        SetTextColor(hdc, RGB(50, 50, 50));
        SetBkMode(hdc, TRANSPARENT);

        const wchar_t* text = L"Launcher";
        SIZE textSize;
        GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);
        int xCenter = (ps.rcPaint.right - textSize.cx) / 2;

        TextOut(hdc, xCenter, 10, text, wcslen(text));*/

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
        if (dis->CtlType == ODT_BUTTON)
        {

            //// Draw the button
            // FillRect(dis->hDC, &dis->rcItem, (HBRUSH)(COLOR_BTNFACE + 1));
            // SetBkMode(dis->hDC, TRANSPARENT);

            //// Set the text color based on the button state
            // if (dis->itemState & ODS_SELECTED) {
            //	SetTextColor(dis->hDC, RGB(255, 255, 255));
            // }
            // else {
            //	SetTextColor(dis->hDC, RGB(0, 0, 0));
            // }

            // SelectObject(dis->hDC, buttonFont);
            // SetBkMode(dis->hDC, TRANSPARENT);
            //// Draw the button text
            // DrawText(dis->hDC, L"Hook fovScale", -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            //// Center the button after creation
            // RECT rcClient;
            // GetClientRect(windowHandle, &rcClient);
            // int buttonWidth = 150;
            // int buttonHeight = 50;

            //// Calculate centered position
            // int x = (rcClient.right - buttonWidth) / 2;

            // SetWindowPos(newButton, nullptr, x, (rcClient.bottom - buttonHeight) / 10, 0, 0, SWP_NOZORDER |
            // SWP_NOSIZE);
        }
        return TRUE;
    }
    case WM_DESTROY: PostQuitMessage(0); break;
    default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    default: break;
    }
    return (INT_PTR)FALSE;
}

BOOL InjectDLL(DWORD aProcessId, LPCSTR apDllPath)
{
    if (!aProcessId || !std::filesystem::exists(apDllPath))
        return FALSE;

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcessId);

    if (!process)
    {
        MessageBoxA(windowHandle, std::string("Failed to open process: " + GetLastError()).c_str(), "Error", MB_OK);
        return FALSE;
    }

    LPVOID processAddress = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    LPVOID remoteString = (LPVOID)VirtualAllocEx(process, nullptr, strlen(apDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

    if (WriteProcessMemory(process, remoteString, (LPVOID)apDllPath, strlen(apDllPath) + 1, nullptr))
    {
        CreateRemoteThread(process, nullptr, NULL, (LPTHREAD_START_ROUTINE)processAddress, remoteString, NULL, nullptr);
    }

    CloseHandle(process);

    return TRUE;
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (pKeyboard->vkCode == VK_ESCAPE)
            {
                std::cout << "Escape key pressed";
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

BOOL ResizeButton(const std::vector<Button>::iterator& acIterator, const std::wstring& acWideText)
{
    LPWSTR windowText = (LPWSTR)acWideText.c_str();

    if (acIterator != buttons.end() && windowText != acIterator->text && !acIterator->text.empty())
    {
        HDC hdc = GetDC(windowHandle);
        SIZE textSize;
        GetTextExtentPoint32W(hdc, acWideText.c_str(), static_cast<int>(acWideText.length()), &textSize);
        ReleaseDC(windowHandle, hdc);

        int padding = 15;
        int buttonWidth = textSize.cx + padding * 2;
        int buttonHeight = textSize.cy + padding;

        RECT rcClient;
        GetClientRect(windowHandle, &rcClient);

        const int x = (rcClient.right - buttonWidth) / 2;
        const int y = static_cast<int>(buttons.size()) * (buttonHeight + 10);

        SetWindowTextW(acIterator->handle, windowText);
        acIterator->text = windowText;

        SetWindowPos(acIterator->handle, nullptr, x, y, buttonWidth, buttonHeight, SWP_NOZORDER | SWP_NOMOVE);

        RedrawWindow(acIterator->handle, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

        return TRUE;
    }

    return FALSE;
}
