// Launcher.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Launcher.h"
#include <Resource.h>
#include <iostream>
#include <thread>
#include "Log.h"

constexpr auto MAX_LOADSTRING = 100;

HINSTANCE hInst;                                // current instance
CHAR szTitle[MAX_LOADSTRING];                  // The title bar text
CHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
#if _DEBUG
	SetupConsole();
#endif

	// Initialize global strings
	LoadStringA(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringA(hInstance, IDC_LAUNCHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
	    return FALSE;
	}

	HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_LAUNCHER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
	    if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
	    {
	        TranslateMessage(&msg);
	        DispatchMessage(&msg);
	    }
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAUNCHER));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_LAUNCHER);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	wchar_t text[255]{ '\0' };

	GetWindowThreadProcessId(hWnd, &dwProcessId);

	auto iter = std::find_if(buttons.begin(), buttons.end(), [&](const Button& acButton) { return acButton.windowHandle == hWnd || acButton.processId == dwProcessId || acButton.handle == hWnd; });

	if (!hWnd || !IsWindowVisible(hWnd) || iter != buttons.end() || !SendMessageW(hWnd, WM_GETTEXT, sizeof(text), reinterpret_cast<LPARAM>(text)))
		return TRUE;

	std::wstring wideText(text);

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideText[0], (int)wideText.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wideText[0], (int)wideText.size(), &strTo[0], size_needed, NULL, NULL);

	if (ResizeButton(iter, wideText))
		return TRUE;

	std::wcout << strTo.c_str() << L"\n";

	// Get the size of the text
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
	const int& y = static_cast<int>(buttons.size()) * (buttonHeight + 10);

	HWND buttonHandle = CreateWindowW(
		L"BUTTON",
		wideText.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x, y,
		buttonWidth, buttonHeight,
		windowHandle,
		(HMENU)dwProcessId,
		(HINSTANCE)GetWindowLongPtr(windowHandle, GWLP_HINSTANCE),
		NULL);

	if (!buttonHandle)
		std::wcout << L"Button creation failed: " << GetLastError() << L"\n";

	Button* newButton = new Button{wideText, dwProcessId, buttonHandle, hWnd};
	buttons.emplace_back(*newButton);

	return TRUE;
}

static void WindowCheckThread()
{
	while(true)
	{
		for (auto iter = buttons.begin(); iter != buttons.end();)
		{
			if(!IsWindowVisible(iter->windowHandle))
			{
				DestroyWindow(iter->handle);
				iter = buttons.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		//EnumWindows(EnumWindowsProc, NULL);

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	windowHandle = CreateWindowA(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (!windowHandle)
	{
	  return FALSE;
	}

	EnumWindows(EnumWindowsProc, NULL);
	//std::thread(WindowCheckThread).detach();

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

			auto pIter = std::find_if(buttons.begin(), buttons.end(), [&](const Button& acButton) { return (int)acButton.processId == wmId || acButton.handle == hWnd; });

			if(pIter != buttons.end())
			{
				char buffer[MAX_PATH];
				GetCurrentDirectoryA(MAX_PATH, buffer);
				std::string currentDirectory = buffer;

				currentDirectory = currentDirectory.substr(0, currentDirectory.substr(0, currentDirectory.find_last_of('\\')).find_last_of('\\')).append(LAUNCHER_DLL_PATH);

				InjectDLL(pIter->processId, (LPCSTR)currentDirectory.c_str());
			}

	        switch (wmId)
	        {
	        case IDM_ABOUT:
	            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	            break;
	        case IDM_EXIT:
	            DestroyWindow(hWnd);
	            break;
	        default:
	            return DefWindowProc(hWnd, message, wParam, lParam);
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
	case WM_DRAWITEM: {
		LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
		if (dis->CtlType == ODT_BUTTON) 
		{


			//// Draw the button
			//FillRect(dis->hDC, &dis->rcItem, (HBRUSH)(COLOR_BTNFACE + 1));
			//SetBkMode(dis->hDC, TRANSPARENT);

			//// Set the text color based on the button state
			//if (dis->itemState & ODS_SELECTED) {
			//	SetTextColor(dis->hDC, RGB(255, 255, 255));
			//}
			//else {
			//	SetTextColor(dis->hDC, RGB(0, 0, 0));
			//}

			//SelectObject(dis->hDC, buttonFont);
			//SetBkMode(dis->hDC, TRANSPARENT);
			//// Draw the button text
			//DrawText(dis->hDC, L"Hook fovScale", -1, &dis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			//// Center the button after creation
			//RECT rcClient;
			//GetClientRect(windowHandle, &rcClient);
			//int buttonWidth = 150;
			//int buttonHeight = 50;

			//// Calculate centered position
			//int x = (rcClient.right - buttonWidth) / 2;

			//SetWindowPos(newButton, nullptr, x, (rcClient.bottom - buttonHeight) / 10, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		return TRUE;
	}
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	    return (INT_PTR)TRUE;

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

	if(!process)
	{
		MessageBoxA(windowHandle, std::string("Failed to open process: " + GetLastError()).c_str(), "Error", MB_OK);
		return FALSE;
	}

	LPVOID processAddress = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	LPVOID remoteString = (LPVOID)VirtualAllocEx(process, nullptr, strlen(apDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

	if(WriteProcessMemory(process, remoteString, (LPVOID)apDllPath, strlen(apDllPath) + 1, nullptr))
		CreateRemoteThread(process, nullptr, NULL, (LPTHREAD_START_ROUTINE)processAddress, remoteString, NULL, nullptr);

	CloseHandle(process);

	return TRUE;
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (pKeyboard->vkCode == VK_ESCAPE) {
				std::cout << "Escape key pressed";
			}
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void SetupConsole() noexcept
{
	AllocConsole();
	SetConsoleTitleW(L"MTIP Console");

	FILE* stdinFile;
	FILE* stdoutFile;
	FILE* stderrFile;

	// Redirect stdin
	if (freopen_s(&stdinFile, "conin$", "r", stdin) != 0) {
		std::cerr << "Error redirecting stdin\n";
	}

	// Redirect stdout
	if (freopen_s(&stdoutFile, "conout$", "w", stdout) != 0) {
		std::cerr << "Error redirecting stdout\n";
	}

	// Redirect stderr
	if (freopen_s(&stderrFile, "conout$", "w", stderr) != 0) {
		std::cerr << "Error redirecting stderr\n";
	}

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	//std::cout << "This is red text!" << std::endl;

	//SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//std::cout << "This is green text!" << std::endl;

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	//std::cout << "This is blue text!" << std::endl;

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

BOOL ResizeButton(const std::vector<Button>::iterator& acIterator, const std::wstring& acWideText)
{
	LPWSTR windowText = (LPWSTR)acWideText.c_str();

	if (acIterator != buttons.end() && windowText != acIterator->text && !acIterator->text.empty())
	{
		// Get the size of the text
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
